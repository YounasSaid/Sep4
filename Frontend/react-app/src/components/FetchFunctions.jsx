import {
  useEffect,
  useState,
  createContext,
  useContext,
  Suspense,
} from "react";
import "./css/FetchFunktions.css";

export default function SensorData() {
  const [temp, setTemp] = useState(null);
  const [soil, setSoil] = useState(null);
  const [hum, setHum] = useState(null);
  const [light, setLight] = useState(null);
  const [height, setHeight] = useState(null);
  const [leaf_count, setLeafCount] = useState(null);

  const fetchData = async (type, setter) => {
    try {
      const res = await fetch(
        `http://98.71.68.49:5000/api/measurement/latest?type=${type}`, //henter seneste data
      );
      const data = await res.json();
      setter(data.value);
    } catch (error) {
      console.log(error);
    }
  };
  useEffect(() => {
    fetchData("temp", (value) => setTemp(value.toFixed(2)));
    fetchData("soil", (value) => setSoil((value/1023).toFixed(2)));
    fetchData("hum", (value) => setHum((value).toFixed(2)));
    fetchData("light", (value) => setLight((value/1023).toFixed(2)));
    fetchData("height", (value) => setHeight(value.toFixed(2)));
    //fetchData("leaf_count", (value) => setLeafCount(value.toFixed(2)));
  }, []);

  return (
    <div className="sensor-data">
      <h1>Sensor Data:</h1>
      <p>Temperature: {temp}°C</p>
      <p>Soil Moisture: {soil}%</p>
      <p>Humidity: {hum}%</p>
      <p>Light: {light} lux</p>
      <p>Height: {height} cm</p>
    </div>
  );
}

export async function SendHeight(type, value) {
  try {
    const response = await fetch(
      "http://98.71.68.49:5000/api/measurement",
      {
        method: "POST",
        headers: { "content-type": "application/json" },
        body: JSON.stringify({
          type: type,
          value: value,
          timestamp: new Date().toISOString(),
        }),
      },
    );

    if (!response.ok) {
      throw new Error(`Server error: ${response.status}`);
    }
    const data = await response.json();
    return data;
  } catch (error) {
    console.error("Error", error);
    throw error;
  }
}
