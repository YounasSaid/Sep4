import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import "./index.css";
import App from "./App.jsx";
import { RouterProvider, createHashRouter } from "react-router-dom";
import { createHashRouter } from "create-router";

const router = createHashRouter([
  {
    path: "/",
    element: <App />,
  },
  /*,
  {
    path: "/about",
    element: <h1>About</h1>,
  } */
]);

createRoot(document.getElementById("root")).render(
  <StrictMode>
    <RouterProvider router={router} />
  </StrictMode>,
);
