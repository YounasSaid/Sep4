import pandas as pd
import pickle
import os
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score

MODEL_PATH = "plant_growth_model.pkl"
DATA_PATH = "plant_growth_data.csv"

# Kategoriske kolonner der skal one-hot encodes
CATEGORICAL = ["Soil_Type", "Water_Frequency", "Fertilizer_Type"]
NUMERICAL = ["Sunlight_Hours", "Temperature", "Humidity"]
TARGET = "Growth_Milestone"


def _prepare_data():
    """Indlæs CSV og one-hot encode kategoriske features."""
    df = pd.read_csv(DATA_PATH)
    df = pd.get_dummies(df, columns=CATEGORICAL, drop_first=True)
    feature_cols = [c for c in df.columns if c != TARGET]
    return df[feature_cols], df[TARGET], feature_cols


def train_plant_model(model_type="logistic", test_size=0.2):
    """Træn klassifikationsmodel på lærerens plantevækst-datasæt."""
    if not os.path.exists(DATA_PATH):
        raise FileNotFoundError(f"{DATA_PATH} mangler. Placer filen i ML-mappen.")

    X, y, feature_cols = _prepare_data()

    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=test_size, random_state=42
    )

    if model_type == "forest":
        model = RandomForestClassifier(n_estimators=100, random_state=42)
    else:
        model = LogisticRegression(max_iter=1000, random_state=42)

    model.fit(X_train, y_train)
    y_pred = model.predict(X_test)

    metrics = {
        "accuracy": round(accuracy_score(y_test, y_pred), 4),
        "precision": round(precision_score(y_test, y_pred, zero_division=0), 4),
        "recall": round(recall_score(y_test, y_pred, zero_division=0), 4),
        "f1": round(f1_score(y_test, y_pred, zero_division=0), 4),
        "train_size": len(X_train),
        "test_size": len(X_test),
    }

    # Gem model + feature-navne så predict kender kolonne-rækkefølgen
    with open(MODEL_PATH, "wb") as f:
        pickle.dump({"model": model, "feature_cols": feature_cols}, f)

    return metrics


def predict_growth(soil_type, sunlight_hours, water_frequency,
                   fertilizer_type, temperature, humidity):
    """Forudsig om planten når Growth_Milestone (0 eller 1)."""
    if not os.path.exists(MODEL_PATH):
        return {"error": "Model ikke trænet endnu. Kald /api/plant/train først."}

    with open(MODEL_PATH, "rb") as f:
        bundle = pickle.load(f)

    model = bundle["model"]
    feature_cols = bundle["feature_cols"]

    # Byg én-rækkes DataFrame med samme one-hot encoding som træning
    row = {
        "Sunlight_Hours": sunlight_hours,
        "Temperature": temperature,
        "Humidity": humidity,
    }
    # One-hot encode kategoriske værdier manuelt
    for col in feature_cols:
        if col.startswith("Soil_Type_"):
            row[col] = 1 if col == f"Soil_Type_{soil_type}" else 0
        elif col.startswith("Water_Frequency_"):
            row[col] = 1 if col == f"Water_Frequency_{water_frequency}" else 0
        elif col.startswith("Fertilizer_Type_"):
            row[col] = 1 if col == f"Fertilizer_Type_{fertilizer_type}" else 0

    df_input = pd.DataFrame([row])[feature_cols]
    prediction = int(model.predict(df_input)[0])
    probability = model.predict_proba(df_input)[0].tolist()

    return {
        "inputs": {
            "soil_type": soil_type,
            "sunlight_hours": sunlight_hours,
            "water_frequency": water_frequency,
            "fertilizer_type": fertilizer_type,
            "temperature": temperature,
            "humidity": humidity,
        },
        "growth_milestone": prediction,
        "probability": {
            "no_milestone": round(probability[0], 4),
            "milestone_reached": round(probability[1], 4),
        },
    }
