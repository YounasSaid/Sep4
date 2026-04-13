import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";

export function Menu()
    {
    return (
        <div id="RightMenu">
          <Link className="NavBut" to="/IntroPage">IOT</Link>
          <Link className="NavBut" to="/IntroPage2">MAL</Link>
        </div>
      ) ;
    }
