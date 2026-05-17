import { useEffect, useState } from "react";
import "./css/Jordfugtighed.css";

function Jordfugtighed() {
  const [soil, setSoil] = useState(null);
  const [soilMin, setSoilMin] = useState(null);
  const [soilMax, setSoilMax] = useState(null);

  const apiStr = "http://4.223.137.178:5000/api/plants/1/measurements/";

  const hentData = async (type, setter) => {
    try {
      const res = await fetch(`${apiStr}latest?type=${type}`, {
        method: "GET",
        headers: {
          "content-type": "application/json",
          "X-API-Key": localStorage.token || "",
        },
      });
      const data = await res.json();
      if (data.value !== undefined) setter(data.value);
    } catch (error) {
      console.log(`Fejl ved hentning af ${type}:`, error);
    }
  };

  useEffect(() => {
    hentData("soil", (value) => setSoil(value / 1023));
    hentData("soil_min", (value) => setSoilMin(value / 1023));
    hentData("soil_max", (value) => setSoilMax(value / 1023));
  }, []);

  const erOk = soil !== null && soilMin !== null && soilMax !== null
    && soil >= soilMin && soil <= soilMax;

  return (
    <div className="jordfugtighed-side">
      <h1>Jordfugtighed</h1>

      <p>Optimal range: {soilMin !== null ? (soilMin * 100).toFixed(0) + "%" : "Henter..."} - {soilMax !== null ? (soilMax * 100).toFixed(0) + "%" : "Henter..."}</p>

      {soil !== null ? (
        <div>
          <p>Nuværende jordfugtighed: {(soil * 100).toFixed(2)}%</p>
          <p className={erOk ? "status-ok" : "status-fejl"}>
            {erOk ? "✅ Inden for optimalt interval" : "⚠️ Uden for optimalt interval"}
          </p>
        </div>
      ) : (
        <p>Henter data...</p>
      )}
    </div>
  );
}

export default Jordfugtighed;