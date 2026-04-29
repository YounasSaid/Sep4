import {
  useEffect,
  useState,
  createContext,
  useContext,
  Suspense,
} from "react";
import "./css/Startpage.css";
import SensorData from "./FetchFunctions";

export function StartPage() {
  return (
   
      <SensorData />
    
  );
}
