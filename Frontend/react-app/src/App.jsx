import { use, useEffect, useState } from "react";
import { BrowserRouter, HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import "./App.css";
import { GlobalContext } from "./components/GlobalContext.jsx"
import { Routing } from "./components/Routing.jsx"
import { StartPage } from "./components/StartPage.jsx";
import { Navbar } from "./components/Navbar.jsx";
import { Menu } from "./components/Menu.jsx";

// ------------------------------------------------------------------------------------------

export function GlobalProvider({ children }) {
  const [GraphType, setGraphType] = useState('Luft');

  const value = { GraphType, setGraphType };

  return (
    <GlobalContext.Provider value={value}>
      {children}
    </GlobalContext.Provider>
  );
}

// ------------------------------------------------------------------------------------------

function App() {
  return (
    <GlobalProvider> {/* Avoid Props Drilling */}
      <HashRouter>
        <Navbar />
        <Menu />
        <Routing />
      </HashRouter>
    </GlobalProvider>
  );
}

export default App;

// ------------------------------------------------------------------------------------------
