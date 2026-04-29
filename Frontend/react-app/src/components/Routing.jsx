import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { BrowserRouter, HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import { Charts } from "./SensorChart.jsx"
import SensorData from './FetchFunctions.jsx';
import { NewData } from './SendData.jsx';
import { StartPage } from './StartPage.jsx';

export function Routing() {

  return (
    <div id="Routing">
      <Routes>
        <Route path="/" element={<StartPage />} />
        <Route path="/SendData" element={<NewData />} />
        <Route path="/Charts" element={<Charts />} />
      </Routes>
    </div>
  );
}
