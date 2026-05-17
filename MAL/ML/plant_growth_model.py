import pandas as pd
import pickle
import os
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score

MODEL_PATH = "plant_growth_model.pkl"
DATA_PATH = "greenhouse_data.csv"

# Features der matcher Arduino-sensorerne (uden co2 og soil_moisture)
FEATURES = ["temperature", "humidity", "light"]
TARGET = "growth_milestone"

# Optimale grænser for plantevækst
OPTIMAL = {
    "temperature": (18, 28),
    "humidity": (40, 70),
    "light": (500, float("inf")),
}


def _prepare_data():
    """Indlæs drivhus-CSV, rename kolonner, og generer growth_milestone label."""
    df = pd.read_csv(DATA_PATH, sep=";", decimal=",")

    # Rename til simple navne
    df = df.rename(columns={
        "greenhous_temperature_celsius": "temperature",
        "greenhouse_humidity_percentage": "humidity",
        "greenhouse_illuminance_lux": "light",
    })

    # Behold kun de kolonner vi bruger
    df = df[FEATURES].dropna()

    # Generer label: 1 = alle sensorværdier er inden for optimale grænser
    df[TARGET] = (
        df["temperature"].between(*OPTIMAL["temperature"])
        & df["humidity"].between(*OPTIMAL["humidity"])
        & (df["light"] >= OPTIMAL["light"][0])
    ).astype(int)

    return df[FEATURES], df[TARGET]


def train_plant_model(model_type="logistic", test_size=0.2):
    """Træn klassifikationsmodel på drivhusdata."""
    if not os.path.exists(DATA_PATH):
        raise FileNotFoundError(f"{DATA_PATH} mangler. Placer filen i ML-mappen.")

    X, y = _prepare_data()

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

    with open(MODEL_PATH, "wb") as f:
        pickle.dump({"model": model, "features": FEATURES}, f)

    return metrics


def _lux_to_adc(lux_value, max_lux):
    """Omregn lux-vaerdi til ADC 0-1023 skala (som Arduinoen bruger)."""
    return round((lux_value / max_lux) * 1023, 2)


def get_optimal_ranges():
    """Beregn optimale min/max vaerdier for alle sensortyper.

    For temperatur og luftfugtighed: analyser datasaettet og find
    10. og 90. percentil af de raekker hvor vaekstforholdene er gode
    (growth_milestone == 1). Vaerdierne er i samme enhed som Arduinoen
    sender (grader C og %).

    For lys: beregn fra datasaettet i lux, derefter omregn til ADC 0-1023
    skala saa det matcher Arduinoens raa sensorvaerdier.

    For jordfugtighed: datasaettet indeholder ingen soil-kolonne, saa
    vi bruger faste vaerdier i ADC 0-1023 format.
    """
    if not os.path.exists(DATA_PATH):
        raise FileNotFoundError(f"{DATA_PATH} mangler.")

    # Indlaes fuld data for at finde max lux (til omregning)
    df_full = pd.read_csv(DATA_PATH, sep=";", decimal=",")
    df_full = df_full.rename(columns={
        "greenhous_temperature_celsius": "temperature",
        "greenhouse_humidity_percentage": "humidity",
        "greenhouse_illuminance_lux": "light",
    })
    max_lux = df_full["light"].max()

    X, y = _prepare_data()

    # Kun raekker med gode vaekstforhold
    good = X[y == 1]

    if good.empty:
        return {
            "temp_min": OPTIMAL["temperature"][0],
            "temp_max": OPTIMAL["temperature"][1],
            "hum_min": OPTIMAL["humidity"][0],
            "hum_max": OPTIMAL["humidity"][1],
            "light_min": _lux_to_adc(OPTIMAL["light"][0], max_lux),
            "light_max": 1023.0,
            "soil_min": 300.0,
            "soil_max": 700.0,
        }

    ranges = {
        # Temperatur: fra datasaettet (i grader C, matcher Arduino)
        "temp_min": round(good["temperature"].quantile(0.10), 2),
        "temp_max": round(good["temperature"].quantile(0.90), 2),
        # Luftfugtighed: fra datasaettet (i %, matcher Arduino)
        "hum_min": round(good["humidity"].quantile(0.10), 2),
        "hum_max": round(good["humidity"].quantile(0.90), 2),
        # Lys: fra datasaettet, omregnet fra lux til ADC 0-1023 (matcher Arduino)
        "light_min": _lux_to_adc(good["light"].quantile(0.10), max_lux),
        "light_max": _lux_to_adc(good["light"].quantile(0.90), max_lux),
        # Jordfugtighed: ingen data, faste vaerdier i ADC 0-1023 format (matcher Arduino)
        "soil_min": 300.0,
        "soil_max": 700.0,
    }

    return ranges


def predict_growth(temperature, humidity, light):
    """Forudsig om vækstforholdene er gode (1) eller dårlige (0)."""
    if not os.path.exists(MODEL_PATH):
        return {"error": "Model ikke trænet endnu. Kald /api/plant/train først."}

    with open(MODEL_PATH, "rb") as f:
        bundle = pickle.load(f)

    model = bundle["model"]

    df_input = pd.DataFrame([{
        "temperature": temperature,
        "humidity": humidity,
        "light": light,
    }])[FEATURES]

    prediction = int(model.predict(df_input)[0])
    probability = model.predict_proba(df_input)[0].tolist()

    # Tjek hver sensor mod optimale grænser
    sensor_status = {
        "temperature": {
            "value": temperature,
            "optimal_range": "18-28°C",
            "ok": OPTIMAL["temperature"][0] <= temperature <= OPTIMAL["temperature"][1],
        },
        "humidity": {
            "value": humidity,
            "optimal_range": "40-70%",
            "ok": OPTIMAL["humidity"][0] <= humidity <= OPTIMAL["humidity"][1],
        },
        "light": {
            "value": light,
            "optimal_range": ">500 lux",
            "ok": light >= OPTIMAL["light"][0],
        },
    }

    return {
        "growth_milestone": prediction,
        "probability": {
            "no_milestone": round(probability[0], 4),
            "milestone_reached": round(probability[1], 4),
        },
        "sensor_status": sensor_status,
    }
