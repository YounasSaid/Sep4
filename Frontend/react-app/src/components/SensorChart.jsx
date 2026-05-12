import {
  useEffect,
  useState,
} from "react";

import "./css/SensorChart.css";

import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  Tooltip,
  CartesianGrid,
  ResponsiveContainer,
} from "recharts";

import { DateTimeInput } from "./DateTimeInput";
import { PlantIdPicker } from "./PlantIdPicker";

const apiKey = "bDFRlq8S3KME4SosGXqtUQOUOcik7fxS";
const apiStr =
  "http://4.223.137.178:5000/api/plants/1/measurements/?type=${type}";

const TypeDK = new Map([
  ["temp", "Temperatur"],
  ["soil", "Jord Fugtighed"],
  ["hum", "Luftfugtighed"],
  ["light", "Lys"],
  ["height", "Højde"],
]);

export default function SensorChart() {
  const [data, setData] = useState([]);
  const [range, setRange] = useState("all");
  const [type, setType] = useState("temp");

  // Data From DateTimeInput Component -------------------
  const [startDT, setStartDT] = useState("");
  const [slutDT, setSlutDT] = useState("");

  // Call Back Function From Component
  function handleStartDTFomComponent(data) {
    setStartDT(data);
  }

  // Call Back Function From Component
  function handleSlutDTFomComponent(data) {
    setSlutDT(data);
  }

  // Data From PlantIdPicker Component -------------------
  const [plantId, setPlantId] = useState("");

  // Call Back Function From Component
  function handlePlantIdFomComponent(data) {
    setPlantId(data);
  }
 
  // Test Functioner
  useEffect(() => {
    console.log("StartDT", startDT) ;
  }, [startDT]);
 
  useEffect(() => {
    console.log("SlutDT", slutDT) ;
  }, [slutDT]);

  useEffect(() => {
    console.log("plantId", plantId) ;
  }, [plantId]);

 // ------------------------------------------------------

  useEffect(() => {
    fetchData();
  }, [type]);

  const fetchData = async () => {
    try {
      console.log("Fetching");
      const res = await fetch(`${apiStr}?type=${type}`, {
        method: "GET",
        headers: {
          "content-type": "application/json",
          "X-API-Key": apiKey,
        },
      });

      if (!res.ok) {
        throw new Error(`Response status: ${res.status}`);
      }

      const data = await res.json();
      console.log("data", data);

      const formatted = data.map((item) => ({
        time: new Date(item.timestamp).toLocaleTimeString(),
        rawTime: new Date(item.timestamp),
        value: Number(item.value),
      }));

      formatted.sort((a, b) => a.rawTime - b.rawTime);

      setData(formatted);
    } catch (error) {
      console.error(`Error Fetching ${type} :`, error.message);
    }
  };
  //filter
  const filteredData = data.filter((item) => {
    if (range === "all") return true;

    const now = new Date();
    const diff = now - item.rawTime;

    if (range === "1h") return diff <= 3600000;
    else if (range === "6h") return diff <= 21600000;
    else if (range === "24h") return diff <= 86400000;

    return true;
  });

  return (
    <div className="site">
      <h2>Vælg en måletype</h2>

      <div className="buttons">
        <select onChange={(e) => setType(e.target.value)}>
          <option value="temp">Temperatur</option>
          <option value="soil">Jord Fugtighed</option>
          <option value="hum">Luftfugtighed</option>
          <option value="light">Lys</option>
          <option value="height">Højde</option>
        </select>
      </div>

      <h3>{TypeDK.get(type)}</h3>

      <ResponsiveContainer>
        <LineChart data={filteredData}>
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis dataKey="time" />
          <YAxis dataKey="value" />
          <Tooltip />
          <Line type="monotone" dataKey="value" stroke="red" dot={false} />
        </LineChart>
      </ResponsiveContainer>

      <div style={{ textAlignLast: "center" }}>
        <DateTimeInput 
          sendStartDTToParent = { handleStartDTFomComponent }
          sendSlutDTToParent = { handleSlutDTFomComponent } 
          />
        <PlantIdPicker 
          sendPlantIdToParent = { handlePlantIdFomComponent }
          />
        {/* Submit On Click */}
        <button className="SubmitBut">Hent</button>
      </div>
    </div>
  );
}

export function Charts() {
  return (<SensorChart />);
}
