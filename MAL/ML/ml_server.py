from flask import Flask, jsonify, request
from functools import wraps
import os
from model import train_model, predict
from fetch_data import fetch_and_save
from plant_growth_model import train_plant_model, predict_growth

app = Flask(__name__)

API_KEY = os.getenv("API_KEY")
API_URL = os.getenv("API_URL", "http://localhost:5000")

# Standard plante-id hvis ikke angivet
DEFAULT_PLANT_ID = 7


def require_api_key(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        if not API_KEY:
            return jsonify({"error": "API_KEY env var er ikke sat på serveren"}), 500
        if request.headers.get("X-API-Key") != API_KEY:
            return jsonify({"error": "Unauthorized: Manglende eller forkert X-API-Key header"}), 401
        return f(*args, **kwargs)
    return decorated


def _cloud_headers():
    return {"X-API-Key": API_KEY, "Content-Type": "application/json"}


# ── Eksisterende sensor-model (regression: forudsig plantehøjde) ──

# POST /api/predict - forudsig plantehøjde baseret på alle sensorværdier
@app.route("/api/predict", methods=["POST"])
@require_api_key
def get_prediction():
    data = request.get_json()
    soil_moisture = data.get("soil_moisture")
    temperature = data.get("temperature")
    humidity = data.get("humidity")
    light = data.get("light")

    if None in [soil_moisture, temperature, humidity, light]:
        return jsonify({
            "error": "Alle felter er påkrævet: soil_moisture, temperature, humidity, light"
        }), 400

    result = predict(soil_moisture, temperature, humidity, light)
    return jsonify(result)

# POST /api/train - træn modellen med nyeste data
# Valgfri query param: ?type=linear (default) eller ?type=forest
@app.route("/api/train", methods=["POST"])
@require_api_key
def train():
    # Hent friske data fra serveren før træning
    try:
        fetch_and_save()
    except Exception as e:
        return jsonify({"error": f"Kunne ikke hente data: {str(e)}"}), 500

    if not os.path.exists("training_data.csv"):
        return jsonify({"error": "Ingen træningsdata."}), 400

    model_type = request.args.get("type", "linear")
    score = train_model(model_type=model_type)
    return jsonify({
        "message": "Model trænet",
        "model_type": model_type,
        "score": score
    })


# ── Plant Growth model (klassifikation: forudsig vækstforhold) ──

# POST /api/plant/train - træn klassifikationsmodel på drivhusdata
# Valgfri query param: ?type=logistic (default) eller ?type=forest
@app.route("/api/plant/train", methods=["POST"])
@require_api_key
def train_plant():
    model_type = request.args.get("type", "logistic")
    try:
        metrics = train_plant_model(model_type=model_type)
    except FileNotFoundError as e:
        return jsonify({"error": str(e)}), 400
    except Exception as e:
        return jsonify({"error": f"Træning fejlede: {str(e)}"}), 500

    return jsonify({
        "message": "Plant growth model trænet",
        "model_type": model_type,
        "metrics": metrics,
    })

# POST /api/plant/predict - forudsig vækstforhold baseret på sensordata
@app.route("/api/plant/predict", methods=["POST"])
@require_api_key
def predict_plant():
    data = request.get_json()

    required = ["temperature", "humidity", "light"]

    missing = [f for f in required if f not in data]
    if missing:
        return jsonify({
            "error": f"Manglende felter: {', '.join(missing)}",
            "required": required,
        }), 400

    result = predict_growth(
        temperature=float(data["temperature"]),
        humidity=float(data["humidity"]),
        light=float(data["light"]),
    )
    return jsonify(result)


# POST /api/plant/evaluate - hent seneste sensordata, kør predict, send til cloud
# Valgfri query param: ?plantId=7 (default 7)
@app.route("/api/plant/evaluate", methods=["POST"])
@require_api_key
def evaluate_and_send():
    import requests as http_requests

    plant_id = request.args.get("plantId", DEFAULT_PLANT_ID, type=int)
    headers = _cloud_headers()

    # 1. Hent seneste sensordata fra C# serveren (nye endpoints + typenavne)
    # Arduino sender: temp, hum, light, soil
    sensor_map = {"temp": "temperature", "hum": "humidity", "light": "light"}
    sensor_values = {}

    for api_type, model_key in sensor_map.items():
        try:
            resp = http_requests.get(
                f"{API_URL}/api/plants/{plant_id}/measurements/latest",
                params={"type": api_type},
                headers=headers,
            )
            if resp.status_code == 404:
                return jsonify({"error": f"Ingen data for '{api_type}' paa plante {plant_id}"}), 400
            resp.raise_for_status()
            sensor_values[model_key] = resp.json()["value"]
        except Exception as e:
            return jsonify({"error": f"Kunne ikke hente '{api_type}': {str(e)}"}), 500

    # 2. Kør predict
    result = predict_growth(
        temperature=sensor_values["temperature"],
        humidity=sensor_values["humidity"],
        light=sensor_values["light"],
    )

    if "error" in result:
        return jsonify(result), 400

    # 3. Send vækstforhold-vurdering til C# serveren
    try:
        http_requests.post(
            f"{API_URL}/api/plants/{plant_id}/measurements",
            headers=headers,
            json={
                "type": "growth_conditions",
                "value": result["probability"]["milestone_reached"],
            },
        ).raise_for_status()
    except Exception as e:
        return jsonify({"error": f"Kunne ikke sende til cloud: {str(e)}"}), 500

    return jsonify({
        "message": f"Vaekstforhold-vurdering sendt til cloud for plante {plant_id}",
        "prediction": result,
    })


# GET /api/plant/growth-rate?plantId=7 - beregn reel vækstrate fra højdemålinger
@app.route("/api/plant/growth-rate", methods=["GET"])
@require_api_key
def growth_rate():
    import requests as http_requests
    from datetime import datetime

    plant_id = request.args.get("plantId", DEFAULT_PLANT_ID, type=int)
    headers = _cloud_headers()

    # Hent højdemålinger
    try:
        resp = http_requests.get(
            f"{API_URL}/api/plants/{plant_id}/measurements",
            params={"type": "height", "limit": 100},
            headers=headers,
        )
        if resp.status_code == 404:
            return jsonify({"error": f"Ingen hoejdemaalinger for plante {plant_id}"}), 404
        resp.raise_for_status()
        data = resp.json()
    except Exception as e:
        return jsonify({"error": f"Kunne ikke hente data: {str(e)}"}), 500

    if len(data) < 2:
        return jsonify({"error": "Mindst 2 hoejdemaalinger kraeves for at beregne vaekstrate"}), 400

    # Sorter efter tid (ældste først)
    data.sort(key=lambda x: x["timestamp"])

    # Beregn vækstrate mellem hver måling
    rates = []
    for i in range(1, len(data)):
        t0 = datetime.fromisoformat(data[i - 1]["timestamp"].replace("Z", "+00:00"))
        t1 = datetime.fromisoformat(data[i]["timestamp"].replace("Z", "+00:00"))
        h0 = data[i - 1]["value"]
        h1 = data[i]["value"]

        hours = (t1 - t0).total_seconds() / 3600
        if hours > 0:
            mm_per_day = ((h1 - h0) / hours) * 24
            rates.append({
                "from": data[i - 1]["timestamp"],
                "to": data[i]["timestamp"],
                "height_from": h0,
                "height_to": h1,
                "hours": round(hours, 1),
                "mm_per_day": round(mm_per_day, 2),
            })

    # Samlet statistik
    all_rates = [r["mm_per_day"] for r in rates]
    first = data[0]
    last = data[-1]
    total_hours = (
        datetime.fromisoformat(last["timestamp"].replace("Z", "+00:00"))
        - datetime.fromisoformat(first["timestamp"].replace("Z", "+00:00"))
    ).total_seconds() / 3600
    total_growth = last["value"] - first["value"]

    return jsonify({
        "plant_id": plant_id,
        "total_measurements": len(data),
        "first_height_mm": first["value"],
        "latest_height_mm": last["value"],
        "total_growth_mm": round(total_growth, 2),
        "total_hours": round(total_hours, 1),
        "avg_mm_per_day": round((total_growth / total_hours) * 24, 2) if total_hours > 0 else 0,
        "current_rate_mm_per_day": rates[-1]["mm_per_day"] if rates else 0,
        "rate_history": rates,
    })


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5001)
