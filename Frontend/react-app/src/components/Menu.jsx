import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import "./css/Menu.css"
import SensorData from './FetchFunctions';

export function Menu() {
  return (
    <div className='menu'>
      <Link className="navBut" to="/IOT">IOT</Link>
      <Link className="navBut" to="/Charts">Sensor Charts</Link>
    </div>
  );
}
