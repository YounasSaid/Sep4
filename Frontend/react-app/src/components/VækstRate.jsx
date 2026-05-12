import { useEffect, useState } from 'react';
import '../css/VækstRate.css';

function VækstRate() {

  // Her gemmer vi data fra ML serveren
  const [resultat, setResultat] = useState(null);

  // Tidspunkt for seneste opdatering
  const [sidstOpdateret, setSidstOpdateret] = useState('');

  // API nøgle til serveren
  const apiKey = 'bDFRlq8S3KME4SosGXqtUQOUOcik7fxS';

  // Denne funktion henter data og sender til ML
  const hentOgForudsig = async () => {
    try {

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

      const data = await mlRes.json();
      setResultat(data);

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
  }, []);

  // Vis loading mens vi venter på data
  if (!resultat) {
    return <div className="loading">Henter data...</div>;
  }

  const erGod = resultat.growth_milestone === 1;
  const procent = Math.round(resultat.probability.milestone_reached * 100);

  return (
    <div className="vaekst-side">

      {/* Titel */}
      <h1 className="titel">🌱 Vækstrate Forudsigelse</h1>

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
          <span>{resultat.sensor_status.temperature.value}°C</span>
          <span>{resultat.sensor_status.temperature.ok ? '✅' : '⚠️'}</span>
          <span>({resultat.sensor_status.temperature.optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>💧 Luftfugtighed</span>
          <span>{resultat.sensor_status.humidity.value}%</span>
          <span>{resultat.sensor_status.humidity.ok ? '✅' : '⚠️'}</span>
          <span>({resultat.sensor_status.humidity.optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>☀️ Lys</span>
          <span>{resultat.sensor_status.light.value} lux</span>
          <span>{resultat.sensor_status.light.ok ? '✅' : '⚠️'}</span>
          <span>({resultat.sensor_status.light.optimal_range})</span>
        </div>

        <div className="sensor-raekke">
          <span>🌿 CO2</span>
          <span>{resultat.sensor_status.co2.value} ppm</span>
          <span>{resultat.sensor_status.co2.ok ? '✅' : '⚠️'}</span>
          <span>({resultat.sensor_status.co2.optimal_range})</span>
        </div>

      </div>

      {/* Sidst opdateret */}
      <p className="sidst-opdateret">🕐 Sidst opdateret: {sidstOpdateret}</p>

    </div>
  );
}

export default VækstRate;