import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { BrowserRouter, HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";

import { StartPage } from "./StartPage.jsx"
import { IntroPage } from "./IntroPage.jsx"
import { IntroPage2 } from "./IntroPage2.jsx"

export function MainArea()
  {

  return (
    <div id="MainArea" style={{aborder: '2px solid blue'}}>
    <Routes>
      <Route path="/" element={<StartPage />} />
      <Route path="/IntroPage" element={<IntroPage />} />
      <Route path="/IntroPage2" element={<IntroPage2 />} />
    </Routes>
    </div>
    ) ;
    }
