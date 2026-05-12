import "./css/Login.css";
import { useState } from "react";
import { useNavigate } from "react-router";

const API_KEY = import.meta.env.VITE_API_KEY;
const apiLoginStr = "http://4.223.137.178:5000/api/login";

export default function LoginForm() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const navigate = useNavigate();
  async function HandleLogin(e) {
    e.preventDefault();
    if (!username || !password) {
      setError("indsæt venligst et gyldigt brugernavn og password");
      return;
    }

    try {
      const res = await fetch(apiLoginStr, {
        method: "POST",
        headers: {
          "content-type": "application/json",
        },
        body: JSON.stringify({ username:username,password: password }),
      });
      if (res.ok) {
        setUsername("");
        setPassword("");
        setError("");
        navigate("/start");
      }
      else{
        setError("Brugernavnet eller adgangskoden er forkert, prøv igen");
      }
    } catch (error) {
      setError("fejl ved logind, prøv igen senere");
    }
  }
  return (
    <div className="login-container">
      <form className="login-form" onSubmit={HandleLogin}>
        <h2>Login</h2>
        <input
          type="text"
          placeholder="Username"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
        />
        <input
          type="password"
          placeholder="Password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
        />
        <button type="submit">Login</button>
        {error && <p className="error">{error}</p>}
      </form>
    </div>
  );
}
