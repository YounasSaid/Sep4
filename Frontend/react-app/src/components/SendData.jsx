import {
  useEffect,
  useState,
  createContext,
  useContext,
  Suspense,
} from "react";
import { SendData } from "./FetchFunctions";

function SendDataComponent() {
  const [type, setType] = useState("height");
  const [value, setValue] = useState("");

  const submit = async () => {
    try {
      const result = await SendData(type, Number(value));
      console.log("Success:", result);
    } catch (error) {
      console.error("Failed:", error);
    }
  };
  return (
    // her skal html kode være
     <div className="side">
      <div className="indhold">
        <h1 className="titel">Drivhusprojektet</h1>
        <div className="input-boks">
          <label>Indtast højde:</label>
          <input type="number" placeholder="cm" />
        </div>
        <button className="send-knap">Send</button>
      </div>
      <div className="sidebar">
        <a href="#">Mål Højde</a>
        <a href="#">Sensor Grafer</a>
      </div>
    </div>
  );
}

export function NewData() {
  return <div id="NewData"></div>;
}
