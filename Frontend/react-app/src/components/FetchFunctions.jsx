import {
  useEffect,
  useState,
  createContext,
  useContext,
  Suspense,
} from "react";

import "./css/FetchFunktions.css";

const apiKey = "bDFRlq8S3KME4SosGXqtUQOUOcik7fxS";
const apiStr = "http://4.223.137.178:5000/api/plants/1/measurements/";
const apiWaterStr = "http://4.223.137.178:5000/api/plants/1/water/";

export default function SensorData() {
  const [temp, setTemp] = useState(null);
  const [soil, setSoil] = useState(null);
  const [hum, setHum] = useState(null);
  const [light, setLight] = useState(null);
  const [height, setHeight] = useState(null);
  //const [leaf_count, setLeafCount] = useState(null);

  const fetchData = async (type, setter) => {
    try {
      //hent seneste data
      const res = await fetch(`${apiStr}latest?type=${type}`, {
        method: "GET",
        headers: {
          "content-type": "application/json",
          "X-API-Key": apiKey,
        },
      });

      const data = await res.json();

      if (data.value !== undefined) setter(data.value); // If There's A NetWork Error setter() Is Never Called !
    } catch (error) {
      console.log(`Error Fetching ${type} : `, error);
    }
  };

  useEffect(() => {
    fetchData("temp", (value) => setTemp(value.toFixed(2)));
    fetchData("soil", (value) => setSoil((value / 1023).toFixed(2)));
    fetchData("hum", (value) => setHum(value.toFixed(2)));
    fetchData("light", (value) => setLight((value / 1023).toFixed(2)));
    fetchData("height", (value) => setHeight(value.toFixed(2)));
    //fetchData("leaf_count", (value) => setLeafCount(value.toFixed(2)));
  }, []);

  return (
    <div className="sensor-data">
      <h1>Sensor Data:</h1>
      <p>Temperatur: {temp != null ? temp + " °C" : "Ingen Data !"}</p>
      <p>Jord Fugtighed: {soil != null ? soil + " %" : "Ingen Data !"}</p>
      <p>Luftfugtighed: {hum != null ? hum + " %" : "Ingen Data !"}</p>
      <p>Lys: {light != null ? light + " Lux" : "Ingen Data !"}</p>
      <p>Højde: {height != null ? height + " cm" : "Ingen Data !"}</p>
    </div>
  );
}

export async function SendHeight(type, value) {
  try {
    const response = await fetch(`${apiStr}`, {
      method: "POST",
      headers: {
        "content-type": "application/json",
        "X-API-Key": apiKey,
      },
      body: JSON.stringify({
        type: type,
        value: value,
        timestamp: new Date().toISOString(),
      }),
    });

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

export async function SendWater(plantId, amount) {
  try {
    const response = await fetch(`${apiWaterStr}`, {
      method: "POST",
      headers: {
        "content-type": "application/json",
        "X-API-Key": apiKey
      },
      body: Math.floor(Number(amount)),
    });
    return response.ok
  } catch (error) {
    console.error("Error", error);
    throw error;
  }
}
