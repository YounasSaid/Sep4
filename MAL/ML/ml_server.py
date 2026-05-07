from flask import Flask, jsonify, request
import os
from model import train_model, predict
from fetch_data import fetch_and_save
from plant_growth_model import train_plant_model, predict_growth

app = Flask(__name__)

# ── Eksisterende sensor-model (regression: forudsig plantehøjde) ──

# POST /api/predict - forudsig plantehøjde baseret på alle sensorværdier
@app.route("/api/predict", methods=["POST"])
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


# ── Plant Growth model (klassifikation: forudsig Growth_Milestone) ──

# POST /api/plant/train - træn klassifikationsmodel på lærer-datasæt
# Valgfri query param: ?type=logistic (default) eller ?type=forest
@app.route("/api/plant/train", methods=["POST"])
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

# POST /api/plant/predict - forudsig Growth_Milestone
@app.route("/api/plant/predict", methods=["POST"])
def predict_plant():
    data = request.get_json()

    required = ["soil_type", "sunlight_hours", "water_frequency",
                 "fertilizer_type", "temperature", "humidity"]

    missing = [f for f in required if f not in data]
    if missing:
        return jsonify({
            "error": f"Manglende felter: {', '.join(missing)}",
            "required": required,
            "valid_values": {
                "soil_type": ["loam", "sandy", "clay"],
                "water_frequency": ["daily", "weekly", "bi-weekly"],
                "fertilizer_type": ["chemical", "organic", "none"],
            }
        }), 400

    result = predict_growth(
        soil_type=data["soil_type"],
        sunlight_hours=float(data["sunlight_hours"]),
        water_frequency=data["water_frequency"],
        fertilizer_type=data["fertilizer_type"],
        temperature=float(data["temperature"]),
        humidity=float(data["humidity"]),
    )
    return jsonify(result)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5001)
