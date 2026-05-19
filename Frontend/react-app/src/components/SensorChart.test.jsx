import { afterEach, expect, test } from "vitest"
import { cleanup, render, screen, within } from '@testing-library/react';
import "@testing-library/jest-dom/vitest"

import { GlobalContext } from './GlobalContext'
import { SensorChart } from "./SensorChart";

afterEach(() => {
  cleanup()
})

describe('SensorChart Component', () => {
  it('SensorChart vises', () => 
    {
    render(
      <GlobalContext.Provider value={{ plantId: 1 }}>
        <SensorChart />
      </GlobalContext.Provider>
      );
    
    expect(screen.getByText(/Vælg en måletype/i)).toBeInTheDocument()
    });

  it('But should render with the correct label', () => 
    {
    render(
      <GlobalContext.Provider value={{ plantId: 1 }}>
        <SensorChart />
      </GlobalContext.Provider>
      );
    
    expect(screen.getByRole('button', { name: 'Hent Data' })).toBeInTheDocument()
    });
  });