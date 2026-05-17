import { useEffect, useState } from 'react';
import './css/VaekstRate.css';

function VaekstRate() {

  // Her gemmer vi data fra ML serveren
  const [resultat, setResultat] = useState(null);

  // Tidspunkt for seneste opdatering
  const [sidstOpdateret, setSidstOpdateret] = useState('');

  // API nøgle til serveren
  const apiKey = 'bDFRlq8S3KME4SosGXqtUQOUOcik7fxS';

  // Denne funktion henter data og sender til ML
  const hentOgForudsig = async () => {
    try {
/*
      // Hent de seneste målinger fra serveren
      const [tempRes, humRes, lightRes, co2Res] = await Promise.all([
        fetch('/api/measurement/latest?type=temperature', { headers: { 'X-API-Key': apiKey } }),
        fetch('/api/measurement/latest?type=humidity', { headers: { 'X-API-Key': apiKey } }),
        fetch('/api/measurement/latest?type=light', { headers: { 'X-API-Key': apiKey } }),
        fetch('/api/measurement/latest?type=co2', { headers: { 'X-API-Key': apiKey } }),
      ]);

      const temp = await tempRes.json();
      const hum = await humRes.json();
      const light = await lightRes.json();
      const co2 = await co2Res.json();

      // Send målingerne til ML serveren
      const mlRes = await fetch('/api/plant/predict', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'X-API-Key': apiKey,
        },
        body: JSON.stringify({
          temperature: temp.value,
          humidity: hum.value,
          light: light.value,
          co2: co2.value,
        }),
      });
      */

      //const data = await mlRes.json();
      const data = `{
        "temperature": 42,
        "temperature_ok": true,
        "temperature_optimal_range": "OK!,
        "humidity": 42,
        "humidity_ok": true,
        "humidity_optimal_range": "OK!,
        "light": 42,
        "light_ok": true,
        "light_optimal_range": "OK!,
        "co2": 42,
        "co2_ok": true,
        "co2_optimal_range": "OK!,
        }` ;console.log("data",data)

      setResultat(data);console.log("resultat",resultat)

      // Gem tidspunkt for opdatering
      const nu = new Date();
      setSidstOpdateret(nu.toLocaleString('da-DK'));

    } catch (error) {
      console.error('Fejl ved hentning af data:', error);
    }
  };

  // Kør funktionen når siden indlæses
  useEffect(() => {
    hentOgForudsig();
  }, [resultat]);

  // Vis loading mens vi venter på data
  if (!resultat) {
    return <div className="loading">Henter data...</div>;
  }

  const erGod = 1 ;//resultat.growth_milestone === 1;
  const procent = 75 ; //Math.round(resultat.probability.milestone_reached * 100);

  return (
    <div className="vaekst-side">

      {/* Titel */}
      <h1 className="titel">🌱 VækstRate Forudsigelse</h1>

      {/* Grøn eller rød indikator */}
      <div className={erGod ? 'indikator groen' : 'indikator roed'}>
        <span>{erGod ? '✅' : '⚠️'}</span>
        <span>{erGod ? `Gode vækstforhold (${procent}%)` : `Dårlige vækstforhold (${procent}%)`}</span>
      </div>

      {/* Progress bar */}
      <div className="progress-bar-baggrund">
        <div
          className="progress-bar-fyld"
          style={{ width: `${procent}%`, backgroundColor: erGod ? '#4caf50' : '#f44336' }}
        />
      </div>
      <p className="procent-tekst">{procent}%</p>

      {/* Sensor tabel */}
      <h2 className="sensor-titel">Baseret på seneste målinger:</h2>
      <div className="sensor-liste">

        <div className="sensor-raekke">
          <span>🌡️ Temperatur</span>
          <span>42{resultat.temperature}°C</span>
          <span>{true || resultat?.temperature_ok ? '✅' : '⚠️'}</span>
          <span>OK!({resultat?.temperature_optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>💧 Luftfugtighed</span>
          <span>56{resultat?.humidity}%</span>
          <span>{false || resultat?.humidity_ok ? '✅' : '⚠️'}</span>
          <span>Warning!({resultat?.humidity_optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>☀️ Lys</span>
          <span>67{resultat?.light} %</span>
          <span>{true || resultat?.light_ok ? '✅' : '⚠️'}</span>
          <span>(OK!{resultat?.light_optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>🌿 CO2</span>
          <span>32{resultat?.co2} ppm</span>
          <span>{true || resultat?.co2_ok ? '✅' : '⚠️'}</span>
          <span>(OK!{resultat?.optimal_range})</span>
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
