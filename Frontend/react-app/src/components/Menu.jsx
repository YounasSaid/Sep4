import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { useLocation, HashRouter, Routes, Route, Link, NavLink } from "react-router";
import "./css/Menu.css"

export function Menu() {
  const location = useLocation();
  if (location.pathname === "/"){
    return null;
  }
  return (
    <div className='menu'>
      <Link className="navBut" to="/start">senesteData</Link>
      <Link className="navBut" to="/WaterControl">Vand </Link>
      <Link className="navBut" to="/SendHeight">Send højde</Link>
      <Link className="navBut" to="/Charts">Grafer</Link>
        <div className="logoutLogo"><a className='lockLink' href="/"><img src="lock.png" alt="logout" className='lock' /></a></div>
    </div>
    
  );
}
