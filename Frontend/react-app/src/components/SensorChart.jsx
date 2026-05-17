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

const apiBaseStr = "http://4.223.137.178:5000/api/plants/";

const TypeDK = new Map([
  ["temp", "Temperatur"],
  ["soil", "Jord Fugtighed"],
  ["hum", "Luftfugtighed"],
  ["light", "Lys"],
  ["height", "Højde"],
]);

let setCurrentLocalDateTimeStr = () =>
  {
  const now = new Date();
  const yesterday = new Date();

  // Default StartDate X Days In The Past
  yesterday.setDate(now.getDate() - 10);

  const year = now.getFullYear();
  const month = String(now.getMonth() + 1).padStart(2, '0'); // Months are zero-based
  const day = String(now.getDate()).padStart(2, '0');

  const year2 = yesterday.getFullYear();
  const month2 = String(yesterday.getMonth() + 1).padStart(2, '0'); // Months are zero-based
  const day2 = String(yesterday.getDate()).padStart(2, '0');
  
  const hours = String(now.getHours()).padStart(2, '0');
  const minutes = String(now.getMinutes()).padStart(2, '0');

  const formattedDateTime = `${year}-${month}-${day}T${hours}:${minutes}`;
  const formattedDateTime2 = `${year2}-${month2}-${day2}T${hours}:${minutes}`;
  console.log(formattedDateTime, formattedDateTime2) ;

  return [ formattedDateTime2, formattedDateTime ] ;
  } ;

export default function SensorChart() {
  const [noData, setNoData] = useState(false);
  const [data, setData] = useState([]);
  const [range, setRange] = useState("all");
  const [type, setType] = useState("temp");

  // Data From DateTimeInput Component -------------------
  const [startDT, setStartDT] = useState(setCurrentLocalDateTimeStr()[0]);
  const [slutDT, setSlutDT] = useState(setCurrentLocalDateTimeStr()[1]);

  // Call Back Function From Component
  function handleStartDTFomComponent(data) {
    setStartDT(data);
  }

  // Call Back Function From Component
  function handleSlutDTFomComponent(data) {
    setSlutDT(data);
  }

  // Data From PlantIdPicker Component -------------------
  const [plantId, setPlantId] = useState("1");

  // Call Back Function From Component
  function handlePlantIdFomComponent(data) {
    setPlantId(data);
  }
 
  // Test Functions
  useEffect(() => {
    console.log("StartDT", startDT) ;
  }, [startDT]);
 
  useEffect(() => {
    console.log("SlutDT", slutDT) ;
  }, [slutDT]);

 // ------------------------------------------------------

  // Fetch Data On Submit
  const handleSubmit = (event) => {
    event.preventDefault(); // Prevents the default form submission behavior
    // Handle form submission logic here
    console.log("handleSubmit",startDT,slutDT,plantId,type) ;
    fetchData();
  };

 // ------------------------------------------------------

  // Fetch When Changing Type
  useEffect(() => {
    fetchData();
  }, [type]);

  // Fetch When Changing PlantID
  useEffect(() => {
    console.log("plantId", plantId) ;
    fetchData();
  }, [plantId]);

  const fetchData = async () => {
    try {
      const FetchStr = `${apiBaseStr}${plantId}/measurements?type=${type}&from=${startDT}Z&to=${slutDT}Z&limit=100000` ;
      console.log("Fetching", FetchStr);
      //const res = await fetch(`${apiBaseStr}${plantId}/measurements/?type=temp&from=2026-04-15T12:26:40Z&to=2026-05-16T18:51:47.951518Z&limit=2250`, {
      const res = await fetch(FetchStr, {
        method: "GET",
        headers: {
          "content-type": "application/json",
          "X-API-Key": localStorage.token || "",
        },
      });

      if (!res.ok) {
        throw new Error(`Response status: ${res.status}`);
      }

      const data = await res.json();
      console.log("RAW Measure data", data);

      // No Data In Choosen DateTime Interval
      if (data.length === 0)
        setNoData(true) ;

      // Too Much Data Here We Need To Reduce
      /*const formatted = data.map((item) => ({
        time: new Date(item.timestamp).toLocaleTimeString(),
        rawTime: new Date(item.timestamp),
        value: Number(item.value)
      }));*/

      // Use every 50th Data Point
      const formatted = data.reduce((acc, item, index) => {
        if (index % 50 === 0) {
          const NewItem = {
          time: new Date(item.timestamp).toLocaleTimeString(),
          rawTime: new Date(item.timestamp),
          value: Number(item.value)
          }
          acc.push(NewItem);
          }
        return acc;
        }, []); 

      // Data Comes With The Newest First We Need Oldest First
      formatted.reverse() ; console.log("formatted reversed",formatted);
      //formatted.sort((a, b) => a.rawTime - b.rawTime);

      setData(formatted);
      console.log("Formatted Measure data", formatted);
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

      <div style={{fontSize: "24px", textAlign: "center"}} className="buttons">
        <select onChange={(e) => setType(e.target.value)}>
          <option value="temp">Temperatur</option>
          <option value="soil">Jord Fugtighed</option>
          <option value="hum">Luftfugtighed</option>
          <option value="light">Lys</option>
          <option value="height">Højde</option>
        </select>
      </div>

      {/*<h3>{TypeDK.get(type)}</h3>*/}

      <ResponsiveContainer>
        <LineChart data={filteredData}>
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis dataKey="time" />
          <YAxis dataKey="value" />
          <Tooltip />
          <Line type="monotone" dataKey="value" stroke="red" dot={false} />
        </LineChart>
      </ResponsiveContainer>
      

      <form onSubmit={handleSubmit}>
      <div style={{ textAlignLast: "center" }}>
        <DateTimeInput 
          defaultTime = {[startDT, slutDT]}
          sendStartDTToParent = { handleStartDTFomComponent }
          sendSlutDTToParent = { handleSlutDTFomComponent } 
          />
        <PlantIdPicker 
          sendPlantIdToParent = { handlePlantIdFomComponent }
          fetchURL = { `${apiBaseStr}` }
          />
        {/* Submit On Click */}
        <button 
          className="SubmitBut"
          type="submit"
          >
            Hent Data
        </button>
      </div>
        </form>
    </div>
  );
}

export function Charts() {
  return (<SensorChart />);
}
