import {
  useEffect,
  useState,
  createContext,
  useContext,
  Suspense,
} from "react";

const apiStr = "http://4.223.137.178:5000/api/plants/1/measurements/";
const apiWaterStr = "http://4.223.137.178:5000/api/plants/1/water/";

export async function SendHeight(type, value) {
  try {
    const response = await fetch(`${apiStr}`, {
      method: "POST",
      headers: {
        "content-type": "application/json",
        
          "X-API-Key": localStorage.token || "",
      },
      body: JSON.stringify({
        type: type,
        value: value,
        timestamp: new Date().toISOString(),
      }),
    });

    if (!response.ok) {
      throw new Error(`Server error: ${response.status}`);
    }
    const data = await response.json();
    return data;
  } catch (error) {
    console.error("Error", error);
    throw error;
  }
}

export async function SendWater(plantId, amount) {
  try {
    const response = await fetch(`${apiWaterStr}`, {
      method: "POST",
      headers: {
        "content-type": "application/json",
        "X-API-Key": localStorage.token || "",
      },
      body: Math.floor(Number(amount)),
    });
    return response.ok
  } catch (error) {
    console.error("Error", error);
    throw error;
  }
}
