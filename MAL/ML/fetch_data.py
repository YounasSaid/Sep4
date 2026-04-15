import requests
import pandas as pd
import os

API_URL = os.getenv("API_URL", "http://localhost:5000")

# Sensortyper vi henter fra serveren
SENSOR_TYPES = ["soil_moisture", "temperature", "humidity", "light"]

def fetch_and_save():
    # Hent sensordata (alle typer: jordfugt, temp, luftfugt, lys)
    sensor_response = requests.get(f"{API_URL}/api/sensordata")
    sensor_response.raise_for_status()
    sensor_data = sensor_response.json()

    # Hent manuelle målinger (højde, blade)
    measurement_response = requests.get(f"{API_URL}/api/measurement")
    measurement_response.raise_for_status()
    measurement_data = measurement_response.json()

    if not sensor_data or not measurement_data:
        print("Ikke nok data endnu. Brug for både sensordata og manuelle målinger.")
        return

    # Sensordata: en række per tidspunkt med alle sensortyper som kolonner
    df_sensor = pd.DataFrame(sensor_data)
    df_sensor["timestamp"] = pd.to_datetime(df_sensor["timestamp"])
    df_sensor = df_sensor.pivot_table(index="timestamp", columns="type", values="value")
    df_sensor = df_sensor.reset_index()

    # Sikr at alle forventede sensorkolonner findes (også hvis en sensortype mangler)
    for sensor_type in SENSOR_TYPES:
        if sensor_type not in df_sensor.columns:
            df_sensor[sensor_type] = None

    # Manuelle målinger
    df_measurements = pd.DataFrame(measurement_data)
    df_measurements["timestamp"] = pd.to_datetime(df_measurements["timestamp"])

    # Rund timestamps ned til nærmeste time for at matche sensor + manuelle data
    df_sensor["hour"] = df_sensor["timestamp"].dt.floor("h")
    df_measurements["hour"] = df_measurements["timestamp"].dt.floor("h")

    # Kombiner sensor og manuelle målinger
    df = pd.merge(
        df_sensor,
        df_measurements[["hour", "heightMm", "leafCount", "healthScore"]],
        on="hour",
        how="inner"
    )

    df = df.drop(columns=["hour", "timestamp"])
    df = df.dropna()

    df.to_csv("training_data.csv", index=False)
    print(f"Gemt {len(df)} rækker til training_data.csv")
    print(f"Kolonner: {list(df.columns)}")

if __name__ == "__main__":
    fetch_and_save()
