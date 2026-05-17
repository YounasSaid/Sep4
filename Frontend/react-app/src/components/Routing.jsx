import {
  useEffect,
  useState,
  createContext,
  useContext,
  Suspense,
} from "react";
import {
  BrowserRouter,
  HashRouter,
  Routes,
  Route,
  Link,
  NavLink,
} from "react-router";
import { Charts } from "./SensorChart.jsx";
import {SendHeight } from "./FetchFunctions.jsx";
import { NewData } from "./SendHeight.jsx";
import { WaterControl } from "./Watercontrol.jsx";
import LoginForm from  "./Login.jsx";
import { VaekstRate2 } from "./VaekstRate.jsx";

export function Routing() {
  return (
    <Routes>
      <Route path="/" element={<LoginForm />} />
      <Route path="/SendHeight" element={<NewData />} />
      <Route path="/Charts" element={<Charts />} />
      <Route path="/WaterControl" element={<WaterControl />} />
      <Route path="/VaekstRate" element={<VaekstRate2 />} />
    </Routes>
  );
}

