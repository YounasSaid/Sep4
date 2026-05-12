import { afterEach, expect, test } from "vitest"
import { cleanup, render, screen, within } from '@testing-library/react';
import "@testing-library/jest-dom/vitest"

import { Navbar } from '../components/Navbar';
import { Routing } from '../components/Routing';

afterEach(() => {
  cleanup()
})

describe('Navbar Component', () => {
  it('Navbar vises', () => 
    {
    render(<Navbar />);
    
    expect(screen.getByText(/Drivhus projekt/i)).toBeInTheDocument()
    });

  /*it('Root forside vises', () => 
    {
    render(<Routing />);
    
    expect(screen.getByText(/Sensor Data:/i)).toBeInTheDocument()
    });*/
  });