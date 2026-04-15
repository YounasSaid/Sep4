import { useEffect, useState, createContext, useContext, Suspense } from 'react'
import "./css/Startpage.css"
import { Menu } from './Menu';
import { Navbar } from './Navbar';

export function StartPage() {
    return (
        <div id="StartPage">
            <Navbar />
            <Menu />
        </div>
    );
}        