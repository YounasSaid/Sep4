from flask import Flask, jsonify, request
import os
from model import train_model, predict
from fetch_data import fetch_and_save

app = Flask(__name__)

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

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5001)
