import { useEffect, useState, createContext, useContext, Suspense } from 'react'

import "./css/SensorChart.css"

import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  Tooltip,
  CartesianGrid,
  ResponsiveContainer
} from "recharts";

import { DateTimeInput } from './DateTimeInput';
import { PlantIDPicker } from './PlantIdPicker';

const apiKey = "bDFRlq8S3KME4SosGXqtUQOUOcik7fxS";
const apiStr = "http://4.223.137.178:5000/api/measurement/";

const TypeDK = new Map([ 
  ["temp", "Temperatur"], ["soil", "Jord Fugtighed"], 
  ["hum", "Luftfugtighed"], ["light", "Lys"], ["height", "Højde"]]) ;

export default function SensorChart() {
  const [data, setData] = useState([]);
  const [range, setRange] = useState("all");
  const [type, setType] = useState("temp"); 

  const fetchData = async () => {
    try {console.log("Fetching")
      const res = await fetch(`${apiStr}?type=${type}`,
        {
          method: 'GET',
          headers: {
            "content-type": "application/json",
            'X-API-Key': apiKey,
          }
        }
      );

      if (!res.ok) 
        {
        throw new Error(`Response status: ${res.status}`);
        }

      const data = await res.json();console.log("data",data);

      const formatted = data.map(item => ({
        time: new Date(item.timestamp).toLocaleTimeString(),
        rawTime: new Date(item.timestamp),
        value: Number(item.value)
        }));

      formatted.sort((a, b) => a.rawTime - b.rawTime);

      setData(formatted);

    } catch (error) {
      console.error(`Error Fetching ${type} : `, error.message);
    }
  }; // End Of fetchData() 
  
  //fetch når type ændres
  /*useEffect(() => {
    fetchData();
  }, [type]);

  //realtime
  useEffect(() => {
    const interval = setInterval(fetchData, 5000);
    return () => clearInterval(interval);
  }, [type]);*/




  useEffect(() => {
      const formatted = [
        {
        time: "5:03:30 pm",
        rawTime: new Date(),
        value: "42"
        },
        {
        time: "5:03:31 pm",
        rawTime: new Date(),
        value: "0"
        },
        {
        time: "5:03:32 pm",
        rawTime: new Date(),
        value: "42"
        }
      ];console.log("formatted",formatted);

      formatted.sort((a, b) => a.rawTime - b.rawTime);

      setData(formatted);
  }, [type]);





  //filter
  const filteredData = data.filter(item => {
    if (range === "all") return true;

    const now = new Date();
    const diff = now - item.rawTime;

    if (range === "1h") return diff <= 3600000;
    else if (range === "6h") return diff <= 21600000;
    else if (range === "24h") return diff <= 86400000;

    return true;
  });

  return (
    <div className='site'>
      
      <h2>Vælg en måletype"</h2>

      <div className='buttons'>
        <button onClick={() => setType("temp")}>Temperatur</button>
        <button onClick={() => setType("soil")}>Jord Fugtighed</button>
        <button onClick={() => setType("hum")}>Luftfugtighed</button>
        <button onClick={() => setType("light")}>Lys</button>
        <button onClick={() => setType("height")}>Højde</button>
      </div>

      {/*{type && (
        <select onChange={(e) => setRange(e.target.value)}> 
          <option value="all">All</option>
          <option value="1h">Last 1 hour</option>
          <option value="6h">Last 6 hours</option>
          <option value="24h">Last 24 hours</option>
        </select>
      )}*/}

      <h3>{TypeDK.get(type)}</h3>

      <ResponsiveContainer>
        <LineChart data={filteredData}>
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis dataKey="time" />
          <YAxis />
          <Tooltip />
          <Line type="monotone" dataKey="value" stroke="red" dot={false} />
        </LineChart>
      </ResponsiveContainer>

      <div style={{textAlignLast:"left"}}>
        <DateTimeInput />
        <PlantIDPicker />
      </div>
    </div>
  );
}

export function Charts() {
  return (<SensorChart />) ;
}