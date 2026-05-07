import {
  useEffect,
  useState,
  createContext,
  useContext,
  Suspense,
} from "react";

import { SendData } from "./FetchFunctions";

import "./css/SendData.css";

function SendDataComponent() {
  const [type, setType] = useState("height");
  const [value, setValue] = useState("");

  const submit = async () => {
    if (!value) {
      console.error("Please enter a value.");
      return;
    }
    try {
      const result = await SendData(type, Number(value));
      console.log("Success:", result);
      setValue("");
    } catch (error) {
      console.error("Failed:", error);
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
    <SendDataComponent />
    );
}
