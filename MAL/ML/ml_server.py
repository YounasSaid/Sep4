from flask import Flask, jsonify, request
import pandas as pd
import os
from model import train_model, predict

app = Flask(__name__)

# GET /api/predict - forudsig vækstrate baseret på sensorværdier
@app.route("/api/predict", methods=["POST"])
def get_prediction():
    data = request.get_json()
    soil_moisture = data.get("soil_moisture")

    if soil_moisture is None:
        return jsonify({"error": "soil_moisture er påkrævet"}), 400

    result = predict(soil_moisture)
    return jsonify(result)

# POST /api/train - træn modellen med nyeste data
@app.route("/api/train", methods=["POST"])
def train():
    if not os.path.exists("training_data.csv"):
        return jsonify({"error": "Ingen træningsdata. Kør fetch_data.py først."}), 400

    score = train_model()
    return jsonify({"message": "Model trænet", "score": score})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5001)
