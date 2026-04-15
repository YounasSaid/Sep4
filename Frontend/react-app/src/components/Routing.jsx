import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { BrowserRouter, HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import { IOT } from "./IotGrapharea.jsx"
import { MAL } from "./MalGrapharea.jsx"

export function Routing() {

  return (
    <div id="Routing">
      <Routes>
        <Route path="/IOT" element={<IOT/>} />
        <Route path="/MAL" element={<MAL/>} />
      </Routes>
    </div>
  );
}
