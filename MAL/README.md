# MAL – Server & ML

## Azure URLs (til resten af gruppen)

```
C# Server:  https://sep4-server.azurewebsites.net
ML Server:  https://sep4-ml.azurewebsites.net
```

Begge er offentlige – I skal ikke have login. Brug dem direkte fra frontend/IoT.

## REST endpoints – C# Server

| Metode | Endpoint | Beskrivelse |
|---|---|---|
| GET | `/api/measurement?type=<type>` | Hent målinger af given type |
| GET | `/api/measurement/latest?type=<type>` | Hent seneste måling af given type |
| POST | `/api/measurement` | Opret ny måling. Body: `{"type":"soil_moisture","value":45.5}` |

**Gyldige `type` værdier:** `soil_moisture`, `temperature`, `humidity`, `light`, `height`, `leaf_count`

## REST endpoints – ML Server

| Metode | Endpoint | Beskrivelse |
|---|---|---|
| POST | `/api/train` | Træn modellen på nyeste data fra C# serveren. Query param: `?type=linear` eller `?type=forest` |
| POST | `/api/predict` | Forudsig plantehøjde. Body: `{"soil_moisture":45,"temperature":22,"humidity":55,"light":500}` |

## Eksempler

```javascript
// Frontend – hent jordfugtigheds-historik
const res = await fetch("https://sep4-server.azurewebsites.net/api/measurement?type=soil_moisture");
const data = await res.json();

// Frontend – få en prediction
const pred = await fetch("https://sep4-ml.azurewebsites.net/api/predict", {
  method: "POST",
  headers: { "Content-Type": "application/json" },
  body: JSON.stringify({ soil_moisture: 45, temperature: 22, humidity: 55, light: 500 })
});
```

```bash
# IoT – send en måling
curl -X POST https://sep4-server.azurewebsites.net/api/measurement \
  -H "Content-Type: application/json" \
  -d '{"type":"soil_moisture","value":45.5}'
```

## Lokal udvikling

```bash
cd MAL
docker compose up
```

Kører C# server på `http://localhost:5000`, ML server på `http://localhost:5001`, Postgres på `localhost:5432`.
