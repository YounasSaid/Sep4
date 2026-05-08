import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import "./css/Menu.css"
import SensorData from './FetchFunctions';

export function Menu() {
  return (
    <div className='menu'>
      <Link className="navBut" to="/WaterControl">Vand </Link>
      <Link className="navBut" to="/SendHeight">Send højde</Link>
      <Link className="navBut" to="/Charts">Grafer</Link>
    </div>
  );
}
