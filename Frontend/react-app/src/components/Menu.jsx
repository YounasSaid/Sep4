import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import "./css/Menu.css"
import { CurrentTemp } from './currenttemp';

export function Menu() {
  return (
    <div className='menu'>
      <div className='temp' title='Current Temperature'> <CurrentTemp /> </div>
      <Link className="navBut" to="/IOT">IOT</Link>
      <Link className="navBut" to="/MAL">MAL</Link>
    </div>
  );
}
