import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { BrowserRouter, HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import { Charts } from "./SensorChart.jsx"
import SendHeight from './FetchFunctions.jsx';
import { NewData } from './SendHeight.jsx';
import { StartPage } from './StartPage.jsx';

export function Routing() {

  return (
    <Routes>
      <Route path="/" element={<StartPage />} />
      <Route path="/SendHeight" element={<NewData />} />
      <Route path="/Charts" element={<Charts />} />
    </Routes>
  );
}
