import { useState, useContext } from "react";
import { SendWater } from "./FetchFunctions.jsx";

import "./css/Watercontrol.css";

import { GlobalContext } from "./GlobalContext.jsx"

function Watercontrol() {
  // Global PlantId Getter / Setter
  const {plantId, setPlantId } 
    = useContext(GlobalContext);

  const [amount, setAmount] = useState("");
  const [message, setMessage] = useState("");
  const [loading, setLoading] = useState(false);

  const handleWater = async () => {
    if (!amount || amount <= 0) {
      setMessage("Indtast et gyldigt antal ml");
      return;
    }
    try {
      setLoading(true);
      await SendWater(plantId, amount);
      setAmount("");
      setMessage("Planten er blevet vandt!");
    } catch (error) {
      console.error("fejl i serveren");
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="Water">
      <h2>Vand planten</h2>
      <div className="input-boks">
        <input
          type="number"
          placeholder="ML vand"
          value={amount}
          onChange={(e) => setAmount(e.target.value)}
        />
        <button className="send-knap" onClick={handleWater} disabled={loading}>
          {loading ? "Vander..." : "Vand plante"}
        </button>
        <p>{message}</p>
      </div>
    </div>
  );
}
export function WaterControl() {
  return (
    <div id="WaterControl">
      <Watercontrol />
    </div>
  );
}
