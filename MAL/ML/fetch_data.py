import requests
import pandas as pd
import os

API_URL = os.getenv("API_URL", "http://localhost:5000")

# Sensortyper vi henter fra serveren (matcher Measurement.Type)
SENSOR_TYPES = ["soil_moisture", "temperature", "humidity", "light"]

# Manuelle måle-typer (plantehøjde osv.)
MANUAL_TYPES = ["height", "leaf_count"]


def fetch_measurements(measurement_type):
    """Hent alle målinger af en given type fra serveren."""
    response = requests.get(f"{API_URL}/api/measurement", params={"type": measurement_type})
    response.raise_for_status()
    return response.json()


def fetch_and_save():
    all_frames = []

    # Hent hver sensortype separat og saml i én DataFrame
    for sensor_type in SENSOR_TYPES + MANUAL_TYPES:
        try:
            data = fetch_measurements(sensor_type)
            if not data:
                print(f"Ingen data for '{sensor_type}'")
                continue

            df = pd.DataFrame(data)
            df["timestamp"] = pd.to_datetime(df["timestamp"])
            # Vi bruger time-granularitet for at kunne merge forskellige typer
            df["hour"] = df["timestamp"].dt.floor("h")
            # Gennemsnit per time (hvis flere målinger samme time)
            df_grouped = df.groupby("hour")["value"].mean().reset_index()
            df_grouped = df_grouped.rename(columns={"value": sensor_type})
            all_frames.append(df_grouped)
        except Exception as e:
            print(f"Fejl ved hentning af '{sensor_type}': {e}")

    if not all_frames:
        print("Ingen data at gemme.")
        return

    # Merge alle typer på 'hour'
    df_merged = all_frames[0]
    for frame in all_frames[1:]:
        df_merged = pd.merge(df_merged, frame, on="hour", how="outer")

    df_merged = df_merged.sort_values("hour").dropna()

    if df_merged.empty:
        print("Ingen rækker hvor alle sensortyper har data. Brug for mere data.")
        return

    df_merged.to_csv("training_data.csv", index=False)
    print(f"Gemt {len(df_merged)} rækker til training_data.csv")
    print(f"Kolonner: {list(df_merged.columns)}")


if __name__ == "__main__":
    fetch_and_save()
