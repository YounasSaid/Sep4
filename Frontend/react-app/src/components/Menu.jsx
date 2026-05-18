import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { useLocation, HashRouter, Routes, Route, Link, NavLink } from "react-router";
import "./css/Menu.css"

import { GlobalContext } from "./GlobalContext.jsx"
import { PlantIdPicker } from "./PlantIdPicker";

const apiBaseStr = "http://4.223.137.178:5000/api/plants/";

export function Menu() {
  // From PlantId Picker To Global Span
  const {plantId, setPlantId } 
    = useContext(GlobalContext);

  // Call Back Function From Component
  function handlePlantIdFomComponent(data) {
    setPlantId(data);
  }

  const location = useLocation();
  if (location.pathname === "/") {
    return null;
    }

  const handleLogout = () => {
    localStorage.removeItem("token");
    };

  return (
    <div className='menu'>
      <div className="logoutLogo"><a className='lockLink' href="/" onClick={handleLogout}><img src="lock.png" alt="logout" className='lock' /></a></div>
      <Link className="navBut" to="/VaekstRate">VækstRate</Link>
      <Link className="navBut" to="/WaterControl">Vand </Link>
      <Link className="navBut" to="/SendHeight">Send højde</Link>
      <Link className="navBut" to="/Charts">Grafer</Link>

      <PlantIdPicker 
          sendPlantIdToParent = { handlePlantIdFomComponent }
          fetchURL = { `${apiBaseStr}` }
          />
    </div>
    );
  }
