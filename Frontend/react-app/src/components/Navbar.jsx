import "./css/Navbar.css";
import { useLocation} from "react-router";

export function Navbar() {
    const location = useLocation();
  if (location.pathname === "/"){
    return null;
  }
  return (
    <div className="navbar">
      <div className="logo">Drivhus projekt</div>
    </div>
  );
}
