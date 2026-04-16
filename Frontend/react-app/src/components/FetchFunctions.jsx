import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import "./css/FetchFunktions.css"

export default function SensorData() {
  const [temperature, setTemperature] = useState(null);
  const [soilMoisture, setSoilMoisture] = useState(null);
  const [humidity, setHumidity] = useState(null);
  const [light, setLight] = useState(null);
  const [height, setHeight] = useState(null);

  const fetchData = async (type, setter) => {
    try {
      const res = await fetch(
        `https://sep4-server.azurewebsites.net/api/measurement/latest?type=${type}` //henter seneste data
      );
      const data = await res.json();
      setter(data.value);
    } catch (error) {
      console.log(error);
    }
  };
    useEffect(() => {
    fetchData("temperature", (value) => setTemperature(value.toFixed(2)));
    fetchData("soil_moisture", (value) => setSoilMoisture(value.toFixed(2)));
    fetchData("humidity", (value) => setHumidity(value.toFixed(2)));
    fetchData("light", (value) => setLight(value.toFixed(2)));
    fetchData("height", (value) => setHeight(value.toFixed(2)));
  }, []);

   return (
    <div className='sensor-data'>
      <h1>Sensor Data:</h1>
      <p>Temperature: {temperature}°C</p>
      <p>Soil Moisture: {soilMoisture}%</p>
      <p>Humidity: {humidity}%</p>
      <p>Light: {light}</p>
      <p>Height: {height} cm</p>
    </div>
  );
}