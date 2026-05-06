import requests
import pandas as pd
import os
from datetime import datetime, timezone

API_URL = os.getenv("API_URL", "http://98.71.68.49:5000")

SENSOR_TYPES = ["soil", "temp", "hum", "light"]
MANUAL_TYPES = ["height"]
OUTPUT_FILE = "training_data.csv"


def fetch_measurements(measurement_type, from_timestamp=None):
    params = {"type": measurement_type, "limit": 10000}
    if from_timestamp:
        params["from"] = from_timestamp.isoformat()
    response = requests.get(f"{API_URL}/api/measurement", params=params)
    response.raise_for_status()
    return response.json()


def fetch_and_save():
    # Find seneste timestamp i eksisterende CSV
    from_timestamp = None
    if os.path.isfile(OUTPUT_FILE):
        df_existing = pd.read_csv(OUTPUT_FILE, parse_dates=["minute"])
        from_timestamp = df_existing["minute"].max().to_pydatetime()
        print(f"Henter data fra {from_timestamp}")

    all_frames = []

    for sensor_type in SENSOR_TYPES:
        try:
            data = fetch_measurements(sensor_type, from_timestamp)
            if not data:
                print(f"Ingen data for '{sensor_type}'")
                continue

            df = pd.DataFrame(data)
            df["timestamp"] = pd.to_datetime(df["timestamp"])
            df["minute"] = df["timestamp"].dt.floor("min")
            df_grouped = df.groupby("minute")["value"].mean().reset_index()
            df_grouped = df_grouped.rename(columns={"value": sensor_type})

            print(f"{sensor_type}: {len(df_grouped)} minutter, "
                  f"{df_grouped['minute'].min()} → {df_grouped['minute'].max()}")

            all_frames.append(df_grouped)
        except Exception as e:
            print(f"Fejl ved hentning af '{sensor_type}': {e}")

    if not all_frames:
        print("Ingen data at gemme.")
        return

    df_merged = all_frames[0]
    for frame in all_frames[1:]:
        df_merged = pd.merge(df_merged, frame, on="minute", how="outer")

    df_merged = df_merged.sort_values("minute")

    tilgængelige = [s for s in SENSOR_TYPES if s in df_merged.columns]
    df_merged[tilgængelige] = df_merged[tilgængelige].ffill().bfill()
    df_merged = df_merged.dropna()

    # Tilføj til eksisterende CSV og fjern dubletter
    if os.path.isfile(OUTPUT_FILE):
        df_existing = pd.read_csv(OUTPUT_FILE, parse_dates=["minute"])
        df_merged = pd.concat([df_existing, df_merged])
        df_merged = df_merged.drop_duplicates(subset=["minute"])
        df_merged = df_merged.sort_values("minute")

    df_merged.to_csv(OUTPUT_FILE, index=False)
    print(f"Gemt {len(df_merged)} rækker i alt til {OUTPUT_FILE}")
    print(f"Kolonner: {list(df_merged.columns)}")


if __name__ == "__main__":
    fetch_and_save()