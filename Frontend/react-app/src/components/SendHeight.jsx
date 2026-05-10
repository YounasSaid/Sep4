import {
  useEffect,
  useState,
  createContext,
  useContext,
  Suspense,
} from "react";
import { SendHeight } from "./FetchFunctions";
import "./css/SendHeight.css";

function SendHeightComponent() {
  const [type, setType] = useState("height");
  const [value, setValue] = useState("");

  const submit = async () => {
    if (!value) {
      console.error("Please enter a value.");
      return;
    }
    try {
      const result = await SendHeight(type, Number(value));
      console.log("Success:", result);
      setValue("");
    } catch (error) {
    }
  };
  
  return (
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
  );
}

export function NewData() {
  return (
    <div id="NewData">
      <SendHeightComponent />
    </div>
  );
}
