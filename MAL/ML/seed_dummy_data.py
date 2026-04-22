"""Seed Azure database med dummy træningsdata til ML modellen.

Genererer 30 timers syntetisk sensor- og højde-data der simulerer
karse-vækst over tid. Kaldes én gang for at give modellen noget at
træne på.
"""
import requests
import random
import datetime

BASE = "https://sep4-server.azurewebsites.net/api/measurement"
random.seed(42)

start = datetime.datetime.utcnow() - datetime.timedelta(hours=30)

for i in range(30):
    # Karse-vækst: starter på ~1mm, vokser ca. 0.15mm/time
    height = 1.0 + i * 0.15 + random.uniform(-0.3, 0.3)
    soil = 40 + random.uniform(-5, 10)
    temp = 22 + random.uniform(-2, 3)
    hum = 55 + random.uniform(-5, 10)
    light = 400 + random.uniform(-100, 300)

    for type_, value in [
        ("soil_moisture", soil),
        ("temperature", temp),
        ("humidity", hum),
        ("light", light),
        ("height", round(height, 2)),
    ]:
        r = requests.post(BASE, json={"type": type_, "value": value}, timeout=30)
        if r.status_code != 201:
            print(f"Fejl {type_}: {r.status_code}")

print("30 timers data sendt til databasen.")
for type_ in ["soil_moisture", "temperature", "humidity", "light", "height"]:
    r = requests.get(BASE, params={"type": type_}, timeout=30)
    print(f"  {type_}: {len(r.json())} rækker")
