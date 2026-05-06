# MAL – Server & ML

## Azure URLs (til resten af gruppen)

Alt kører på én Azure VM med docker-compose.

**Fast IP:** `98.71.68.49`
**DNS:** `sep4-greenhouse.northeurope.cloudapp.azure.com`

| Service | URL | Port |
|---|---|---|
| C# REST API | `http://98.71.68.49:5000` | 5000 |
| C# Socket Server (til IoT) | `98.71.68.49:23` | 23 |
| ML Server | `http://98.71.68.49:5001` | 5001 |

## 🔐 Authentication

**ALLE kald til C# serveren kræver en API nøgle.** Spørg Younas om nøglen – den bliver ikke committet til Git.

### HTTP requests
Send nøglen som `X-API-Key` header på alle `/api/*` endpoints:

```javascript
fetch("http://98.71.68.49:5000/api/measurement?type=soil_moisture", {
  headers: { "X-API-Key": "<nøglen-fra-younas>" }
});
```

### Socket connection (IoT)
Send `AUTH:<nøgle>;` som første besked når socket forbindelsen er åbnet, **før** I sender målinger:

```c
// Lige efter wifi_command_create_TCP_connection
char auth_msg[64];
sprintf(auth_msg, "AUTH:%s;", API_KEY);
wifi_command_TCP_transmit((uint8_t*)auth_msg, strlen(auth_msg));
// Først herefter må I sende målinger som "soil_moisture,45.5;"
```

Hvis nøglen mangler eller er forkert lukker serveren forbindelsen.

### Public endpoints (ingen auth)
- `GET /` – health check ("GreenHouseApi is running")
- `GET /swagger` – API dokumentation

## REST endpoints – C# Server

| Metode | Endpoint | Beskrivelse |
|---|---|---|
| GET | `/api/measurement?type=<type>&limit=N` | Hent målinger (default 20 nyeste, max via `limit`) |
| GET | `/api/measurement/latest?type=<type>` | Hent seneste måling |
| GET | `/api/measurement/aggregate?type=<type>&start=...&secondsPerMeasurement=...&count=...` | Hent min/max/avg over tidsperioder |
| POST | `/api/measurement` | Opret ny måling. Body: `{"type":"soil_moisture","value":45.5}` |

**Gyldige `type` værdier:** `soil_moisture`, `temperature`, `humidity`, `light`, `height`, `leaf_count`

## REST endpoints – ML Server

| Metode | Endpoint | Beskrivelse |
|---|---|---|
| POST | `/api/train` | Træn modellen på nyeste data fra C# serveren |
| POST | `/api/predict` | Forudsig plantehøjde |

ML serveren kalder selv C# serveren med sin egen API nøgle. Frontend kalder ML serveren direkte uden auth lige nu.

## Lokal udvikling

1. Kopier `.env.example` til `.env` og udfyld med dine egne værdier:
   ```bash
   cp MAL/.env.example MAL/.env
   ```
2. Start stack:
   ```bash
   cd MAL
   docker compose up
   ```

Kører C# server på `http://localhost:5000`, ML server på `http://localhost:5001`, Postgres på `localhost:5432`.

## Eksempel – fuld request

```bash
# Opret en måling
curl -X POST http://98.71.68.49:5000/api/measurement \
  -H "Content-Type: application/json" \
  -H "X-API-Key: <nøglen-fra-younas>" \
  -d '{"type":"soil_moisture","value":45.5}'

# Hent de seneste 100 målinger
curl -H "X-API-Key: <nøglen-fra-younas>" \
  "http://98.71.68.49:5000/api/measurement?type=soil_moisture&limit=100"
```
