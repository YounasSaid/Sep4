import { use, useEffect, useState } from "react";
import { BrowserRouter, HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import "./App.css";

import { GlobalContext } from "./components/GlobalContext.jsx"
import { TopBar } from "./components/TopBar.jsx"
import { Menu } from "./components/Menu.jsx"
import { MainArea } from "./components/MainArea.jsx"

// ------------------------------------------------------------------------------------------

export function GlobalProvider({ children }) 
  {
  const [GraphType, setGraphType] = useState('Luft');

  const value = { GraphType, setGraphType };

  return (
    <GlobalContext.Provider value={value}>
      {children}
    </GlobalContext.Provider>
    );
  }

// ------------------------------------------------------------------------------------------

function App() 
{
  return (
  <GlobalProvider> {/* Avoid Props Drilling */}
  <HashRouter>
    <TopBar />

    <div style={{display: 'flex',  width: '100%',  height: '96%'}}>
      <MainArea />
      <Menu />
    </div>
  </HashRouter>
  </GlobalProvider>
  );
}

export default App ;

// ------------------------------------------------------------------------------------------
