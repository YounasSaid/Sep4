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

export default function SensorChart() {
  const [data, setData] = useState([]);
  const [range, setRange] = useState("all");
  const [type, setType] = useState(); 

  const fetchData = async () => {
    try {
      const res = await fetch(
        `http://98.71.68.49:5000/api/measurement/?type=${type}`
      );
      const json = await res.json();

      const formatted = json.map(item => ({
        time: new Date(item.timestamp).toLocaleTimeString(),
        rawTime: new Date(item.timestamp),
        value: Number(item.value)
      }));

      formatted.sort((a, b) => a.rawTime - b.rawTime);

      setData(formatted);
    } catch (err) {
      console.error(err);
    }
  };

  //fetch når type ændres
  useEffect(() => {
    fetchData();
  }, [type]);

  //realtime
  useEffect(() => {
    const interval = setInterval(fetchData, 5000);
    return () => clearInterval(interval);
  }, [type]);

  //filter
  const filteredData = data.filter(item => {
    if (range === "all") return true;

    const now = new Date();
    const diff = now - item.rawTime;

    if (range === "1h") return diff <= 3600000;
    if (range === "6h") return diff <= 21600000;
    if (range === "24h") return diff <= 86400000;

    return true;
  });

  return (
    <div className='site'>

      <div className='buttons'>
        <button onClick={() => setType("temp")}>Temperature</button>
        <button onClick={() => setType("hum")}>Humidity</button>
        <button onClick={() => setType("light")}>Light</button>
      </div>

      <h2>{type ? type.charAt(0).toUpperCase() + type.slice(1) + " Chart" : 'Select a type'}</h2>

      {type && (
        <select onChange={(e) => setRange(e.target.value)}> 
          <option value="all">All</option>
          <option value="1h">Last 1 hour</option>
          <option value="6h">Last 6 hours</option>
          <option value="24h">Last 24 hours</option>
        </select>
      )}

      <ResponsiveContainer>
        <LineChart data={filteredData}>
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis dataKey="time" />
          <YAxis />
          <Tooltip />
          <Line type="monotone" dataKey="value" stroke="red" dot={false} />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
}
export function Charts() {
  return (
    <div id="Charts">
        <SensorChart />
    </div>);
}