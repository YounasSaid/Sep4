import requests
import pandas as pd

API_URL = "http://localhost:5000/api/sensordata"

def fetch_and_save():
    response = requests.get(API_URL)
    response.raise_for_status()
    data = response.json()

    if not data:
        print("Ingen data fundet i databasen.")
        return

    df = pd.DataFrame(data)

    # Omstrukturer: en række per tidspunkt med alle sensortyper som kolonner
    df["timestamp"] = pd.to_datetime(df["timestamp"])
    df_pivot = df.pivot_table(index="timestamp", columns="type", values="value")
    df_pivot = df_pivot.reset_index()
    df_pivot = df_pivot.dropna()

    df_pivot.to_csv("training_data.csv", index=False)
    print(f"Gemt {len(df_pivot)} rækker til training_data.csv")

if __name__ == "__main__":
    fetch_and_save()
