# MAL – Server & ML

## Azure URLs (til resten af gruppen)

Alt kører på én Azure VM med docker-compose (ikke App Service – vi skiftede til VM for at kunne bruge port 23 til socket server).

**Fast IP:** `98.71.68.49`
**DNS:** `sep4-greenhouse.northeurope.cloudapp.azure.com`

| Service | URL | Port |
|---|---|---|
| C# REST API | `http://98.71.68.49:5000` | 5000 |
| C# Socket Server (til IoT) | `98.71.68.49:23` | 23 |
| ML Server | `http://98.71.68.49:5001` | 5001 |

## REST endpoints – C# Server

| Metode | Endpoint | Beskrivelse |
|---|---|---|
| GET | `/api/measurement?type=<type>` | Hent målinger af given type |
| GET | `/api/measurement/latest?type=<type>` | Hent seneste måling af given type |
| POST | `/api/measurement` | Opret ny måling. Body: `{"type":"soil_moisture","value":45.5}` |
| GET | `/api/init-db` | Opret databasetabeller (første gang) |

**Gyldige `type` værdier:** `soil_moisture`, `temperature`, `humidity`, `light`, `height`, `leaf_count`

## REST endpoints – ML Server

| Metode | Endpoint | Beskrivelse |
|---|---|---|
| POST | `/api/train` | Træn modellen på nyeste data fra C# serveren. Query: `?type=linear` eller `?type=forest` |
| POST | `/api/predict` | Forudsig plantehøjde. Body: `{"soil_moisture":45,"temperature":22,"humidity":55,"light":500}` |

## Eksempler

```javascript
// Frontend – hent jordfugtigheds-historik
const res = await fetch("http://98.71.68.49:5000/api/measurement?type=soil_moisture");
const data = await res.json();

// Frontend – få en prediction
const pred = await fetch("http://98.71.68.49:5001/api/predict", {
  method: "POST",
  headers: { "Content-Type": "application/json" },
  body: JSON.stringify({ soil_moisture: 45, temperature: 22, humidity: 55, light: 500 })
});
```

```c
// IoT – forbind til socket server
server_ip = "98.71.68.49";
server_port = 23;
```

```bash
# IoT – alternativt send via HTTP POST
curl -X POST http://98.71.68.49:5000/api/measurement \
  -H "Content-Type: application/json" \
  -d '{"type":"soil_moisture","value":45.5}'
```

## Lokal udvikling

```bash
cd MAL
docker compose up
```

Kører C# server på `http://localhost:5000`, ML server på `http://localhost:5001`, Postgres på `localhost:5432`.
