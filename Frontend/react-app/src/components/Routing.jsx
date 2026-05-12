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
import SendHeight from "./FetchFunctions.jsx";
import { NewData } from "./SendHeight.jsx";
import { StartPage } from "./StartPage.jsx";
import { WaterControl } from "./Watercontrol.jsx";
import LoginForm from  "./Login.jsx";

export function Routing() {
  return (
    <Routes>
      <Route path="/" element={<LoginForm />} />
      <Route path="/start" element={<StartPage />} />
      <Route path="/SendHeight" element={<NewData />} />
      <Route path="/Charts" element={<Charts />} />
      <Route path="/WaterControl" element={<WaterControl />} />
    </Routes>
  );
}

