export async function SendHeight(plantId, type, value) {
  const apiStr = "http://4.223.137.178:5000/api/plants/"+plantId+"/measurements/";

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
  const apiWaterStr = "http://4.223.137.178:5000/api/plants/"+plantId+"/water/";

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
