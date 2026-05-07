import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";

//import styles from '../App.module.css'

export function DateTimeInput()
  {
  return (
    <>
    <h5>Vælg dato og tid Start/Slut</h5>
    <div style={{display:"flex", width:"100%", justifyContent: "space-around"}}>
      <span style={{paddingRight:"12px"}}>
        <label htmlFor="startdaytime">Start :&nbsp;</label>
        <input type="date" id="startdaytime" name="startdaytime" />
      </span>
      <span style={{paddingRight:"12px"}}>
        <label htmlFor="slutdaytime">Slut :&nbsp;</label>
        <input type="time" id="slutdaytime" name="slutdaytime" />
      </span>
      <span style={{paddingRight:"12px"}}>
        <button>Hent</button>
      </span>
    </div>
    </>
    ) ;
  }
