import { useEffect, useState, useContext } from "react";
import "./css/VaekstRate.css";

import { GlobalContext } from "./GlobalContext.jsx"

function VaekstRate() {
  // Global PlantId Getter / Setter
  const {plantId, setPlantId } 
    = useContext(GlobalContext);

  // Her gemmer vi data fra ML serveren
  const [resultat, setResultat] = useState(null);

  // Tidspunkt for seneste opdatering
  const [sidstOpdateret, setSidstOpdateret] = useState("");

  // endpoint for at hente data
  const apiEvaluate = "http://4.223.137.178:5000/api/plants/"+plantId+"/evaluate/"
  const apiStr = "http://4.223.137.178:5000/api/plants/"+plantId+"/measurements/";

  // Sensor data
  const [temp, setTemp] = useState(null);
  const [soil, setSoil] = useState(null);
  const [hum, setHum] = useState(null);
  const [light, setLight] = useState(null);
  const [height, setHeight] = useState(null);

  //status for data hentning
  const [temp_min, setTemp_min] = useState(null);
  const [temp_max, setTemp_max] = useState(null);
  const [hum_min, setHum_min] = useState(null);
  const [hum_max, setHum_max] = useState(null);
  const [soil_min, setSoil_min] = useState(null);
  const [soil_max, setSoil_max] = useState(null);
  const [light_min, setLight_min] = useState(null);
  const [light_max, setLight_max] = useState(null);

  const [growth_conditions, setGrowth_conditions] = useState(null);

  const [evaluate, setEvaluate] = useState(null);

  // Funktion til milegrowth
  const evaluateGrowth = async () => {
    try {
      const response = await fetch(`${apiEvaluate}`, {
        method: "POST",
        headers: {
          "content-type": "application/json",
          "X-API-Key": localStorage.token || "",
        },
        body: JSON.stringify({
        })
      })

      const data = await response.json();
      setEvaluate(data.evaluate);
    } catch (error) {
      console.log("Error evaluating growth:", error);
    }
  };

  evaluateGrowth();

  // Denne funktion henter data og sender til ML
  const hentOgForudsig = async (type, setter) => {
    try {
      //hent seneste data
      const res = await fetch(`${apiStr}latest?type=${type}`, {
        method: "GET",
        headers: {
          "content-type": "application/json",
          "X-API-Key": localStorage.token || "",
        },
      })

      const data = await res.json();

      if (data.value !== undefined) setter(data.value); // If There's A NetWork Error setter() Is Never Called !
    } catch (error) {
      console.log(`Error Fetching ${type} : `, error);
    }
  };

  useEffect(() => {
    hentOgForudsig("temp", (value) => setTemp(value));
    hentOgForudsig("hum", (value) => setHum(value));
    hentOgForudsig("light", (value) => setLight(value / 1023));
    hentOgForudsig("soil", (value) => setSoil(value / 1023));
    hentOgForudsig("height", (value) => setHeight(value));

    hentOgForudsig("temp_min", (value) => setTemp_min(value));
    hentOgForudsig("temp_max", (value) => setTemp_max(value));
    hentOgForudsig("hum_min", (value) => setHum_min(value));
    hentOgForudsig("hum_max", (value) => setHum_max(value));
    hentOgForudsig("soil_min", (value) => setSoil_min(value / 1023));
    hentOgForudsig("soil_max", (value) => setSoil_max(value / 1023));
    hentOgForudsig("light_min", (value) => setLight_min(value / 1023));
    hentOgForudsig("light_max", (value) => setLight_max(value / 1023));

    hentOgForudsig("growth_conditions", (value) => setGrowth_conditions(value * 100));
      
    // Gem tidspunkt for opdatering
    const nu = new Date();
    setSidstOpdateret(nu.toLocaleString("da-DK"));
    }, [plantId]);


     // laver en funktion der tjekker om værdien er inden for det optimale interval og returnere en emoji
     const getEmojiStatus = (value, min, max) => {
     if (value < min || value > max) return "⚠️";
     return "✅";
    };

   const getStatus = (type, value, min, max) => {
     if (value < min || value > max) return "Dårlige vækstforhold";
     return "Gode vækstforhold";
    }
   
  return (
    <div className="vaekst-side">
      {/* Titel */}
      <h1 className="titel">🌱 VækstRate Forudsigelse</h1>

      {/* Grøn eller rød indikator */}
      <div
        className={
          evaluate && evaluate.prediction.growth_conditions
            ? "indikator groen"
            : "indikator roed"
        }
      >
        <span>{evaluate && evaluate.prediction.growth_conditions ? "✅" : "⚠️"}</span>
        <span>
          {evaluate && evaluate.prediction.growth_milestone //viser vækstforhold og procent hvis data er tilgængelig
            ? `Gode vækstforhold`
            : `Dårlige vækstforhold`}
        </span>
      </div>

      {/* Progress bar */} 
      <div className="progress-bar-baggrund">
        <div
          className="progress-bar-fyld"
          style={{
            width: `${growth_conditions ? growth_conditions : 0}%`, // Fyld baren baseret på vækstforhold
            backgroundColor: evaluate && evaluate.prediction.growth_milestone ? "#4caf50" : "#f44336",
          }}
        />
      </div>
      <p className="procent-tekst">
        {growth_conditions ? growth_conditions.toFixed(2) + "%" : "ingen data"}
      </p>

      {/* Sensor tabel */}
      <h2 className="sensor-titel">Baseret på seneste målinger:</h2>
      <div className="sensor-liste">
        <div className="sensor-raekke">
          <span>🌡️ Temperatur</span>
          <span>{temp != null ? temp?.toFixed(2) + "°C" : "Ingen data!"}</span>
          <span>
            {getEmojiStatus(
              temp?.toFixed(2),
              temp_min?.toFixed(2),
              temp_max?.toFixed(2),
            )}
          </span>
          <span>
            {getStatus(
              "temp",
              temp?.toFixed(2),
              temp_min?.toFixed(2),
              temp_max?.toFixed(2),
            )}
          </span>
          <span>
            {temp_min != null && temp_max != null
              ? `(Min: ${temp_min?.toFixed(2)}°C, Max: ${temp_max?.toFixed(2)}°C)`
              : ""}
          </span>
        </div>

        <div className="sensor-raekke">
          <span>💨 Luftfugtighed</span>
          <span>{hum != null ? hum?.toFixed(2) + "%" : "Ingen data!"}</span>
          <span>
            {getEmojiStatus(
              hum?.toFixed(2),
              hum_min?.toFixed(2),
              hum_max?.toFixed(2),
            )}
          </span>
          <span>
            {getStatus(
              "hum",
              hum?.toFixed(2),
              hum_min?.toFixed(2),
              hum_max?.toFixed(2),
            )}
          </span>
          <span>
            {hum_min != null && hum_max != null
              ? `(Min: ${hum_min?.toFixed(2)}%, Max: ${hum_max?.toFixed(2)}%)`
              : ""}
            </span>
        </div>

        <div className="sensor-raekke">
          <span>💧 jordfugtighed</span>
          <span>{soil != null ? soil?.toFixed(2) + "%" : "Ingen data!"}</span>
          <span>
            {getEmojiStatus(
              soil?.toFixed(2),
              soil_min?.toFixed(2),
              soil_max?.toFixed(2),
            )}
          </span>
          <span>
            {getStatus(
              "soil",
              soil?.toFixed(2),
              soil_min?.toFixed(2),
              soil_max?.toFixed(2),
            )}
          </span>
          <span>
            {soil_min != null && soil_max != null
              ? `(Min: ${soil_min?.toFixed(2)}%, Max: ${soil_max?.toFixed(2)}%)`
              : ""}
          </span>
        </div>

        <div className="sensor-raekke">
          <span>☀️ Lys</span>
          <span>{light != null ? light?.toFixed(2) + "%" : "Ingen data!"}</span>
          <span>
            {getEmojiStatus(
              light?.toFixed(2),
              light_min?.toFixed(2),
              light_max?.toFixed(2),
            )}
          </span>
          <span>
            {getStatus(
              "light",
              light?.toFixed(2),
              light_min?.toFixed(2),
              light_max?.toFixed(2),
            )}
          </span>
          <span>
            {light_min != null && light_max != null
              ? `(Min: ${light_min?.toFixed(2)}%, Max: ${light_max?.toFixed(2)}%)`
              : ""}
          </span>
        </div>

        <div className="sensor-raekke">
          <span>📏 højde</span>
          <span>
            {height != null ? height?.toFixed(2) + " mm" : "Ingen data!"}
          </span>
          <span> ! </span>
          <span> ! </span>
        </div>
      </div>

      {/* Sidst opdateret */}
      <p className="sidst-opdateret">🕐 Sidst opdateret: {sidstOpdateret}</p>
    </div>
  );
}

export function VaekstRate2() {
  return (
    <div className="VaekstRate">
      <VaekstRate />
    </div>
  );
}
