import { useEffect, useState, createContext, useContext, Suspense, useRef } from 'react'
import { HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";

import { GlobalContext } from "/src/components/GlobalContext.jsx"

import LiveLineChart from './LiveLineChart'
import LineChart from './LineChart'

export function TopPanel() {
  const { GraphType } = useContext(GlobalContext);

  return (
    <>
    <div id="TopPanel">
        {GraphType} Graph
      <GraphArea />
    </div>
    </>);
}
function GraphArea() {
  return (
    <>
   <h1 style={{ textAlign: 'center' }}>Vite React Line Chart Example</h1>
      <LiveLineChart />
      <LineChart />
  
    </>
  );
}

function Button({ onSmash, children }) {
  return (
    
    <button className='typeBut' onClick={onSmash}>
      {children}
    </button>
  );
}

export function ButPanel() {
  const { setGraphType } = useContext(GlobalContext);
  return (
    <div className='types'>
      <Button onSmash={() => setGraphType('Jordfugtighed')}>
        Jordfugtighed
      </Button>
      <Button onSmash={() => setGraphType('Temp')}>
        Temp
      </Button>
      <Button onSmash={() => setGraphType('Luftfugtighed')}>
        Luftfugtighed
      </Button>
      <Button onSmash={() => setGraphType('Lys')}>
        Lys
      </Button>
      <Button onSmash={() => setGraphType('Højde')}>
        Højde
      </Button>
    </div>
  );
}

export function IOT() {
  return (
    <div id="IOT">
      <TopPanel />
      <ButPanel />
    </div>);
}