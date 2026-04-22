import {
  useEffect,
  useState,
  createContext,
  useContext,
  Suspense,
} from "react";
import { SendData } from "./FetchFunctions";

function App() {
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
    <div></div>
  );
}

export function NewData() {
  return <div id="NewData"></div>;
}
