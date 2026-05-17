# MAL Sprint Dokumentation

## Sprint 3 - Authentication og Secrets

### Hvad blev lavet
- API key authentication på alle HTTP endpoints via custom middleware (ApiKeyMiddleware)
- AUTH handshake på socket-forbindelsen til IoT (Arduino sender "AUTH:<key>;" som første besked)
- API key authentication på ML serveren via Python decorator (@require_api_key)
- Alle hardcoded secrets fjernet fra koden og flyttet til .env filer (gitignored)
- .env.example template oprettet så gruppemedlemmer kan opsætte lokalt miljø

### Teoretisk forklaring

**Hvorfor API key authentication?**
Uden authentication kan enhver der kender serverens IP sende falske sensordata eller hente data fra drivhuset. API key er en simpel men effektiv løsning til machine-to-machine kommunikation, hvor der ikke er en bruger der logger ind. Hver request skal indeholde en hemmelig nøgle i headeren (X-API-Key), og serveren afviser alt uden den rigtige nøgle med HTTP 401 Unauthorized.

**Hvorfor environment variables?**
Secrets (passwords, API keys) må aldrig ligge i kildekoden, fordi Git gemmer al historik. Selv hvis man sletter en secret i et senere commit, kan den stadig findes i git-historikken. Ved at bruge environment variables via .env filer (som er gitignored) sikrer vi at secrets kun eksisterer på de maskiner der har brug for dem, og aldrig ender på GitHub.

**Socket AUTH handshake:**
IoT-enheden (Arduino) kommunikerer over rå TCP sockets, ikke HTTP. Derfor kan vi ikke bruge HTTP headers. I stedet sender Arduino "AUTH:<key>;" som allerførste besked når forbindelsen åbnes. Serveren verificerer nøglen og lukker forbindelsen hvis den er forkert. Først efter godkendelse accepterer serveren sensordata.

### Hvad gik godt
- Authentication blev implementeret på alle lag (server, IoT, ML) i samme sprint
- Secrets blev fjernet fra koden uden at bryde eksisterende funktionalitet
- Auto-deploy via GitHub Actions verificeret og fungerende

### Hvad kan forbedres
- Det gamle database-password ligger stadig i git-historikken (det bruges ikke længere, men er teknisk set synligt)
- SSH-nøgler bør ikke deles mellem gruppemedlemmer - hver person bør have sin egen

---

## Sprint 4 - ML Model med Drivhusdata

### Hvad blev lavet
- ML modellen opdateret fra lærerens syntetiske datasæt (193 rækker) til rigtig drivhusdata (17.563 rækker)
- Features ændret til at matche Arduino-sensorerne: temperature, humidity, light (lux), co2 (ppm)
- Nyt endpoint: POST /api/plant/evaluate - henter selv seneste sensordata, kører predict, og sender resultatet til C# serveren som en measurement (type: "growth_prediction")
- Response inkluderer nu sensor_status med optimale grænser per sensor
- Krav og wireframe udarbejdet til frontend-holdet

### Teoretisk forklaring

**Valg af model: Random Forest Classifier**
Vi valgte Random Forest fordi den fungerer godt med den type data vi har. Random Forest træner 100 uafhængige beslutningstræer, hvor hvert træ ser en tilfældig delmængde af dataen. Den endelige forudsigelse er flertallets afstemning. Dette giver bedre resultater end et enkelt beslutningstræ, fordi tilfældige fejl i individuelle træer udligner hinanden.

Vi sammenlignede med Logistic Regression, som tegner en lineær grænse mellem "gode" og "dårlige" vækstforhold. Den gav 94.8% accuracy men 0% precision/recall - den klassificerede simpelthen alt som "dårlige forhold" fordi datasættet er ubalanceret (de fleste målinger er om natten/vinter hvor lys = 0). Random Forest fangede de nuancerede mønstre og gav 99.97% accuracy med 99.7% F1-score.

**Hvorfor vi genererer labels fra thresholds:**
Datasættet indeholder kun sensordata - ingen kolonne der siger "god vækst" eller "dårlig vækst". Vi definerer derfor selv hvad der er gode vækstforhold baseret på agronomisk viden:
- Temperatur: 18-28°C (de fleste planter trives her)
- Luftfugtighed: 40-70% (for lavt = udtørring, for højt = skimmel)
- Lys: >500 lux (minimum for fotosyntese)
- CO2: <1000 ppm (for højt kan skade planter)

Hvis alle fire er opfyldt samtidig, sættes growth_milestone = 1 (gode forhold), ellers 0.

**Evalueringsmetrikker:**
- Accuracy (99.97%): Andel korrekte forudsigelser ud af alle forudsigelser
- Precision (99.44%): Når modellen siger "gode forhold", hvor ofte har den ret?
- Recall (100%): Hvor mange af de faktisk gode forhold finder modellen?
- F1-score (99.72%): Harmonisk gennemsnit af precision og recall - ét samlet tal for modellens kvalitet

**ML-til-Cloud pipeline (evaluate endpoint):**
ML serveren kan nu selv hente de nyeste sensordata fra C# serveren, køre en forudsigelse, og sende resultatet tilbage som en measurement med type "growth_prediction". Værdien er sandsynligheden for gode vækstforhold (0.0 til 1.0). Dette gør at frontend kun behøver at hente data fra ét sted (C# serveren), og ML-vurderingen kan trigges på kommando eller med jævne mellemrum.

### Hvad gik godt
- Skiftet fra syntetisk til rigtig data gav dramatisk bedre resultater
- Modellens features matcher nu præcis hvad Arduino sender
- Klar API-kontrakt til frontend-holdet med krav, endpoint-beskrivelse og wireframe

### Hvad kan forbedres
- Modellen bruger statiske thresholds til at definere "gode forhold" - ideelt set ville vi bruge faktiske vækstmålinger fra planten
- Datasættet er fra et andet drivhus - med mere data fra vores eget drivhus vil modellen blive endnu bedre
- Auto-evaluering (fx hvert 10. minut) er ikke implementeret endnu - kræver en scheduler eller cron job

---

## Sprint 4 (fortsat) - Vækstrate og Cloud-integration

### Hvad blev lavet
- ML evaluate endpoint opdateret til nyt API-format: `/api/plants/{plantId}/measurements`
- Sensor type-navne opdateret til at matche Arduino: `temp`, `hum`, `light`, `soil`
- CO2 fjernet fra model (Arduino har ikke CO2-sensor)
- Nyt endpoint: `GET /api/plant/growth-rate?plantId=7` - beregner reel vækstrate fra højdemålinger
- Vækstforhold-vurdering (`POST /api/plant/evaluate`) sender nu resultatet til cloud under den rigtige plante

### Teoretisk forklaring

**Vækstforhold vs. vækstrate - to forskellige ting:**
- Vækstforhold (growth_conditions): Er de nuværende sensor-forhold gode for planten? Baseret på ML-klassifikation af temperatur, luftfugtighed og lys mod optimale grænser. Svarer ja/nej med en sandsynlighed.
- Vækstrate (growth_rate): Hvor hurtigt vokser planten faktisk? Beregnet ud fra manuelle højdemålinger over tid. Angives i mm/dag.

Vækstforhold er en forudsigelse (ML), mens vækstrate er en observation (beregning fra data). Sammen giver de et komplet billede: "Forholdene er gode, og planten vokser 10mm/dag" vs. "Forholdene er dårlige, og væksten er faldet til 2mm/dag."

**Evaluate-til-cloud pipeline:**
ML serveren fungerer som en selvstændig service der:
1. Henter de nyeste sensorværdier fra C# serveren (cloud)
2. Kører sin ML-model for at vurdere vækstforhold
3. Poster resultatet tilbage til C# serveren som en measurement (type: `growth_conditions`)

Dette design betyder at frontend kun behøver at tale med C# serveren - den behøver ikke kende til ML serveren overhovedet. Dekobling mellem services gør systemet mere robust og lettere at vedligeholde.

**Vækstrate-beregning:**
Endpointet henter alle højdemålinger for en plante, sorterer dem kronologisk, og beregner vækstraten (mm/dag) mellem hver måling. Det returnerer både en samlet gennemsnitsrate og en historik over alle rater, så frontend kan vise udviklingen over tid. Ingen ML her - det er ren matematik, men det giver den faktiske vækstrate som PB-7 kræver.

### Hvad gik godt
- Rigtige højdemålinger eksisterer allerede (plante 7: 3mm → 25mm over 3 dage)
- API-format matcher nu præcis hvad C# serveren og Arduino bruger
- Både vækstforhold og vækstrate er nu tilgængelige for frontend

### Hvad kan forbedres
- Med nok data over tid kan vi korrelere vækstrate med sensorforhold og bygge en model der forudsiger fremtidig vækstrate
- Auto-evaluering er stadig ikke implementeret (kræver scheduler)
- Højdemålinger afhænger af manuel registrering - kunne automatiseres med kamera-baseret måling
