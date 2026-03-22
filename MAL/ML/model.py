import pandas as pd
import pickle
import os
from sklearn.linear_model import LinearRegression

MODEL_PATH = "model.pkl"

def train_model():
    df = pd.read_csv("training_data.csv")

    # Sprint 1: kun jordfugtighed som feature
    # Senere sprints tilføjer temperatur, luftfugtighed, lys
    X = df[["soil_moisture"]]
    y = df["growth_mm"]

    model = LinearRegression()
    model.fit(X, y)

    score = model.score(X, y)

    with open(MODEL_PATH, "wb") as f:
        pickle.dump(model, f)

    return round(score, 4)

def predict(soil_moisture):
    if not os.path.exists(MODEL_PATH):
        return {"error": "Model ikke trænet endnu. Kald /api/train først."}

    with open(MODEL_PATH, "rb") as f:
        model = pickle.load(f)

    prediction = model.predict([[soil_moisture]])[0]

    return {
        "soil_moisture": soil_moisture,
        "predicted_growth_mm": round(prediction, 2)
    }
