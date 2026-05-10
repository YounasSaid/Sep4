import { useEffect, useState, createContext, useContext, Suspense } from 'react'

import "./css/PlantIdPicker.css"


export function PlantIDPicker() 
  {
  const [plantID, setPlantID] = useState('ID1'); 
  return (
    <div class="PlantPicker" style={{padding:"8px"}}>
    <label>
      Vælg en plante:
      <select
      value={plantID}
      onChange={e => setPlantID(e.target.value)}
    >
      <option value="ID1">#1</option>
      <option value="ID2">#2</option>
      <option value="ID3">#3</option>
    </select>
    </label>
    </div>
    );
  }