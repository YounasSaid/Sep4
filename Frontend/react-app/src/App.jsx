import { useState } from "react";
import {
  BrowserRouter,
  HashRouter,
  Routes,
  Route,
  Link,
  NavLink,
} from "react-router-dom";

import "./App.css";

import { GlobalContext } from "./components/GlobalContext.jsx";
import { Routing } from "./components/Routing.jsx";
import { Navbar } from "./components/Navbar.jsx";
import { Menu } from "./components/Menu.jsx";

// ------------------------------------------------------------------------------------------

export function GlobalProvider({ children }) {
  // define the global values we have access to in the applica
  const [plantId, setPlantId] = useState("1");

  const value = { 
    plantId, setPlantId 
    };

  return (
    <GlobalContext.Provider value={value}>{children}</GlobalContext.Provider>
  );
}

// ------------------------------------------------------------------------------------------

function App() {
  return (
    <HashRouter>
    <GlobalProvider> {/* Avoid Props Drilling (Flems) */}
      <Navbar />
      <Menu />
      <Routing />
    </GlobalProvider>
    </HashRouter>
  );
}

export default App;

// ------------------------------------------------------------------------------------------
