import {
  BrowserRouter,
  HashRouter,
  Routes,
  Route,
  Link,
  NavLink,
} from "react-router";

import LoginForm from  "./Login.jsx";
import { VaekstRate2 } from "./VaekstRate.jsx";
import { WaterControl } from "./Watercontrol.jsx";
import { NewData } from "./SendHeight.jsx";
import { Charts } from "./SensorChart.jsx";

export function Routing() {
  return (
    <Routes>
      <Route path="/" element={<LoginForm />} />
      <Route path="/VaekstRate" element={<VaekstRate2 />} />
      <Route path="/WaterControl" element={<WaterControl />} />
      <Route path="/SendHeight" element={<NewData />} />
      <Route path="/Charts" element={<Charts />} />
    </Routes>
  );
}

