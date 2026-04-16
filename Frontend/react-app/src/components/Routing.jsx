import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { BrowserRouter, HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import { IOT } from "./IotGrapharea.jsx"
import { Charts } from "./SensorChart.jsx"
import SensorData from './FetchFunctions.jsx';

export function Routing() {

  return (
    <div id="Routing">
      <Routes>
        <Route path="/" element={<SensorData/>} />
        <Route path="/IOT" element={<IOT/>} />
        <Route path="/Charts" element={<Charts/>} />
      </Routes>
    </div>
  );
}
