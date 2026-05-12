import { useEffect, useState, createContext, useContext, Suspense } from 'react'

import "./css/PlantIdPicker.css"


export function PlantIdPicker({sendPlantIdToParent}) 
  {
  //const [plantId, setPlantId] = useState('1'); 

 const handlePlantId = e =>
    {
    //setPlantId(e.target.value) ;console.log(e.target.value)
    sendPlantIdToParent(e.target.value) ;
    }

  return (
    <div className="PlantPicker">
    <label>
      Vælg en plante:
      <select
      defaultValue="1"
      onChange={handlePlantId}
    >
      <option value="1">1</option>
      <option value="2">2</option>
      <option value="3">3</option>
    </select>
    </label>
    </div>
    );
  }