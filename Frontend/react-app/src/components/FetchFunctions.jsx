import { useEffect, useState, createContext, useContext, Suspense } from 'react'

// indhenter temprature data fra serveren
export async function GettingCurrnentTemp() {
  const type = "temperature";
  try {
    const datafetch = await fetch(`https://sep4-server.azurewebsites.net/api/measurement?type=${type}`)
    const data = await datafetch.json();
    return (data[0].value.toFixed(2));

  }
  catch (error) {
    console.log(error);
  }
}
// indhenter jordfugtighed data fra serveren
export async function GettingSoilMoisture() {
  const type = "soil_moisture";
  try {
    const datafetch = await fetch(`https://sep4-server.azurewebsites.net/api/measurement?type=${type}`)
    const data = await datafetch.json();
    return (data[0].value);

  }
  catch (error) {
    console.log(error);
  }
}
// indhenter luftfugtighed data fra serveren
export async function GettingHumidity() {
  const type = "humidity";
  try {
    const datafetch = await fetch(`https://sep4-server.azurewebsites.net/api/measurement?type=${type}`)
    const data = await datafetch.json();
    return (data[0].value);

  }
  catch (error) {
    console.log(error);
  }
}
// indhenter lys data fra serveren
export async function GettingLight() {
  const type = "light";
  try {
    const datafetch = await fetch(`https://sep4-server.azurewebsites.net/api/measurement?type=${type}`)
    const data = await datafetch.json();
    return (data[0].value);

  }
  catch (error) {
    console.log(error);
  }
}
// indhenter højde data fra serveren
export async function GettingHeight() {
  const type = "height";
  try {
    const datafetch = await fetch(`https://sep4-server.azurewebsites.net/api/measurement?type=${type}`)
    const data = await datafetch.json();
    return (data[0].value);

  }
  catch (error) {
    console.log(error);
  }
}