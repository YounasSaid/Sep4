import pandas as pd
import pickle
import os
from sklearn.linear_model import LinearRegression
from sklearn.ensemble import RandomForestRegressor

MODEL_PATH = "model.pkl"

# Alle features modellen træner på
FEATURES = ["soil_moisture", "temperature", "humidity", "light"]
TARGET = "height"


def train_model(model_type="linear"):
    df = pd.read_csv("training_data.csv")

    X = df[FEATURES]
    y = df[TARGET]

    if model_type == "forest":
        model = RandomForestRegressor(n_estimators=100, random_state=42)
    else:
        model = LinearRegression()

    model.fit(X, y)
    score = model.score(X, y)

    with open(MODEL_PATH, "wb") as f:
        pickle.dump(model, f)

    return round(score, 4)


def predict(soil_moisture, temperature, humidity, light):
    if not os.path.exists(MODEL_PATH):
        return {"error": "Model ikke trænet endnu. Kald /api/train først."}

    with open(MODEL_PATH, "rb") as f:
        model = pickle.load(f)

    prediction = model.predict([[soil_moisture, temperature, humidity, light]])[0]

    return {
        "inputs": {
            "soil_moisture": soil_moisture,
            "temperature": temperature,
            "humidity": humidity,
            "light": light
        },
        "predicted_height_mm": round(float(prediction), 2)
    }
