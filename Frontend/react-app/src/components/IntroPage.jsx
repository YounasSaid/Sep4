import { useEffect, useState, createContext, useContext, Suspense, useRef } from 'react'
import { HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";

import { GlobalContext } from "/src/components/GlobalContext.jsx"

import LiveLineChart from './LiveLineChart'
import LineChart from './LineChart'

function GraphArea()
  {
  return (

    <div id="GraphArea" 
    style={{ maxWidth: '90%', height: '100%', margin: '0 auto', padding: 20 }}>
      <h1 style={{ textAlign: 'center' }}>
        Live Updating Line Chart
      </h1>
      <LiveLineChart />
      <LineChart />
      <h1 style={{ textAlign: 'center' }}>Vite React Line Chart Example</h1>
      <h1 className="text-center bg-primary p-4">BootStrap Text</h1>
    </div>
    ) ;
  }

export function TopPanel()
    {
    const {GraphType} = useContext(GlobalContext);

    return (
    <div id="TopPanel">
      <div style={{margin:'4px', padding: '4px', fontSize: '24px' }}>
        {GraphType} Graf
      </div>
      <GraphArea />
    </div>) ;
    }

function Button({ onSmash, children }) 
  {
  return (
    <button id="PanelBut" onClick={onSmash}>
      {children}
    </button>
    );
  }

export function ButPanel()
    {
    const {setGraphType} = useContext(GlobalContext);

    return (
        <div id="ButPanel">
          <Button onSmash={() => setGraphType('Luft')}>
            Luft
          </Button>
          <Button onSmash={() => setGraphType('Temp')}>
            Temp
          </Button>
          <Button onSmash={() => setGraphType('Lys')}>
            Lys
          </Button>
        </div>
    ) ;
    }

export function IntroPage()
    {
     return (
    <div id="IntroPage">
        <TopPanel />
        <ButPanel />
    </div>) ;
    }