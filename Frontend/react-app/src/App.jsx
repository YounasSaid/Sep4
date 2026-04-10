import { use, useEffect, useState } from "react";
import reactLogo from "./assets/react.svg";
import viteLogo from "./assets/vite.svg";
import "./App.css";
import Navbar from "./components/Navbar";

function App() {
  const [latestMeasurment, setLatestMeasurement] = useState(null);
  const [allMeasurements, setAllMeasurements] = useState([]);
  const type = "temperature";

  useEffect(() => {
    fetch("http://ipAddress:11111/api/measurement/latest?type=${type}")
      .then((response) => response.json())
      .then((data) => setLatestMeasurement(data))
      .catch((error) =>
        console.error("problem med indhentning af data", error),
      );
  }, []);

  useEffect(() => {
    fetch("http://ipAddress:11111/api/measurement?type=${type}")
      .then((response) => response.json())
      .then((data) => setAllMeasurements(data))
      .catch((error) =>
        console.error("problem med indhentning af data", error),
      );
  }, []);

  return (
    <>
      <div className="container">
        <div className="nav">
          <Navbar />
        </div>

        <div className="temp">
          {latestMeasurment ? `${latestMeasurment.value}°` : "Loading..."}
      
        </div>

        <div className="measurement">
          <h2>Seneste målinger:</h2>

          {!latestMeasurment ? (
            <p>ingen målinger...</p>
          ) : (
            <div>
              {latestMeasurment.type} - {latestMeasurment.value}
            </div>
          )}
        </div>

        <div className="all-data">
          <h2>Alle målinger:</h2>
          {allMeasurements.length === 0 ? (
            <p>ingen målinger...</p>
          ) : (
            allMeasurements.map((m) => (
              <div key={m.id}>
                {m.type} - {m.value} ({m.timestamp})
              </div>
            ))
          )}

            
        </div>
      </div>
    </>
  );
}
export default App;
