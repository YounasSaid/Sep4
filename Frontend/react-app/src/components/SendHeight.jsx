import {
  useState,
  useContext
} from "react";

import { SendHeight } from "./FetchFunctions";

import "./css/SendHeight.css";

import { GlobalContext } from "./GlobalContext.jsx"

function SendHeightComponent() {
  // Global PlantId Getter / Setter
  const {plantId } 
    = useContext(GlobalContext);

  const [type] = useState("height");
  const [value, setValue] = useState("");

  const submit = async () => {
    if (!value) {
      console.error("Please enter a value.");
      return;
    }
    try {
      const result = await SendHeight(plantId, type, Number(value));
      console.log("Success:", result);
      setValue("");
    } catch (err) {console.log("Error:", err.message);}
  };

  return (
    <>
    <h2>Indsend målt højde</h2>
    <div className="input-boks">
      <label>Indtast højde:</label>
      <input
        type="number"
        placeholder="cm"
        value={value}
        onChange={(e) => setValue(e.target.value)}
      />
      <button className="send-knap" onClick={submit}>
        Send
      </button>
    </div>
    </>
  );
}

export function NewData() {
  return (
    <div id="NewData">
      <SendHeightComponent />
    </div>
  );
}
