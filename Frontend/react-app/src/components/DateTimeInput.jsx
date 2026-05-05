import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import { HashRouter, Routes, Route, Link, NavLink } from "react-router-dom";

//import styles from '../App.module.css'

export function DateTimeInput()
  {
  return (
    <div>
    <h1>Vis dato og tid Start/Slut</h1>
    <form>
        <label for="birthdaytime">Start :</label>
        <input type="datetime-local" id="startdaytime" name="startdaytime" />

        <label for="birthdaytime">Slut :</label>
        <input type="datetime-local" id="slutdaytime" name="slutdaytime" />

        <input type="submit" />
    </form>
    </div>
    ) ;
  }
