import { useEffect, useState, createContext, useContext, Suspense } from 'react'

import "./css/PlantIdPicker.css"


export function PlantIdPicker({sendPlantIdToParent, fetchURL}) 
  {
  const [selectData, setSelectData] = useState([{}]);

  useEffect(() => {
    fetchData();
  }, []); // Only Fetch One Time Per Run Of Application

  const fetchData = async () => {
    try {
      console.log("Fetching PlantId", `${fetchURL}`, localStorage.token);
      const res = await fetch(`${fetchURL}`, {
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
      setSelectData(data) ;
      console.log("PlantId data", selectData);

      /*const formatted = data.map((item) => ({
        time: new Date(item.timestamp).toLocaleTimeString(),
        rawTime: new Date(item.timestamp),
        value: Number(item.value),
      }));

      formatted.sort((a, b) => a.rawTime - b.rawTime);*/

      //setData(formatted);
    } catch (error) {
      console.error(`Error Fetching PlantId :`, error.message);
    }
  };

 const handlePlantId = e =>
    {
    //setPlantId(e.target.value) ;console.log(e.target.value)
    sendPlantIdToParent(e.target.value) ;
    }

  return (
    <div className="PlantPicker">
       <span>Vælg en plante:</span>
      <select
      //defaultValue="1"
      onChange={handlePlantId}
      >
      {selectData.map((item) => (
      <option key={item.id} value={item.plantId}>
        {item.plantName}
      </option>
      ))}
    </select>
    </div>
    );
  }