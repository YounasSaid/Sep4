import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import { BrowserRouter, HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";
import './index.css';

import App from './App.jsx'

createRoot(document.getElementById("root")).render(
  <StrictMode>
    <App />
  </StrictMode>,
) 
