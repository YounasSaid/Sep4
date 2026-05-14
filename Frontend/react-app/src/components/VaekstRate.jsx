import { useEffect, useState } from "react";
import "./css/VaekstRate.css";

function VaekstRate() {
  // Her gemmer vi data fra ML serveren
  const [resultat, setResultat] = useState(null);

  // Tidspunkt for seneste opdatering
  const [sidstOpdateret, setSidstOpdateret] = useState("");

  // endpoint for at hente data
  const apiStr = "http://4.223.137.178:5000/api/plants/1/measurements/";

  // Sensor data
  const [temp, setTemp] = useState(null);
  const [soil, setSoil] = useState(null);
  const [hum, setHum] = useState(null);
  const [light, setLight] = useState(null);
  const [height, setHeight] = useState(null);

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
      });

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

    // Gem tidspunkt for opdatering
    const nu = new Date();
    setSidstOpdateret(nu.toLocaleString("da-DK"));
  }, []);

  const erGod = 1; //resultat.growth_milestone === 1;
  const procent = 75; //Math.round(resultat.probability.milestone_reached * 100);

  return (
    <div className="vaekst-side">
      {/* Titel */}
      <h1 className="titel">🌱 VækstRate Forudsigelse</h1>

      {/* Grøn eller rød indikator */}
      <div className={erGod ? "indikator groen" : "indikator roed"}>
        <span>{erGod ? "✅" : "⚠️"}</span>
        <span>
          {erGod
            ? `Gode vækstforhold (${procent}%)`
            : `Dårlige vækstforhold (${procent}%)`}
        </span>
      </div>

      {/* Progress bar */}
      <div className="progress-bar-baggrund">
        <div
          className="progress-bar-fyld"
          style={{
            width: `${procent}%`,
            backgroundColor: erGod ? "#4caf50" : "#f44336",
          }}
        />
      </div>
      <p className="procent-tekst">{procent}%</p>

      {/* Sensor tabel */}
      <h2 className="sensor-titel">Baseret på seneste målinger:</h2>
      <div className="sensor-liste">
        <div className="sensor-raekke">
          <span>🌡️ Temperatur</span>
          <span>{temp != null ? temp?.toFixed(2) + "°C" : "Ingen data!"}</span>
          <span>{true || resultat?.temperature_ok ? "✅" : "⚠️"}</span>
          <span>OK!({resultat?.temperature_optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>💨 Luftfugtighed</span>
          <span>{hum != null ? hum?.toFixed(2) + "%" : "Ingen data!"}</span>
          <span>{false || resultat?.humidity_ok ? "✅" : "⚠️"}</span>
          <span>Warning!({resultat?.humidity_optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>💧 jordfugtighed</span>
          <span>{soil != null ? soil?.toFixed(2) + "%" : "Ingen data!"}</span>
          <span>{true || resultat?.soil_ok ? "✅" : "⚠️"}</span>
          <span>(OK!{resultat?.soil_optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>☀️ Lys</span>
          <span>{light != null ? light?.toFixed(2) + "%" : "Ingen data!"}</span>
          <span>{true || resultat?.light_ok ? "✅" : "⚠️"}</span>
          <span>(OK!{resultat?.light_optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>📏 højde</span>
          <span>{height != null ? height?.toFixed(2) + " cm" : "Ingen data!"}</span>
          <span>{true || resultat?.height_ok ? "✅" : "⚠️"}</span>
          <span>(OK!{resultat?.height_optimal_range})</span>
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
