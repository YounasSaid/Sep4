import { afterEach, expect, test } from "vitest"
import { act, cleanup, render, screen, within } from '@testing-library/react';
import "@testing-library/jest-dom/vitest"

import userEvent from '@testing-library/user-event';

import { Counter } from './Counter';

afterEach(() => {
  cleanup()
  })

describe('Counter Component', () => {
  it('should render with initial count of 0', () => {
    render(<Counter />);

    expect(screen.getByText('Count: 0')).toBeInTheDocument();
  });

  it('should render with custom initial count', () => {
    render(<Counter initialCount={5} />);

    expect(screen.getByText('Count: 5')).toBeInTheDocument();
  });

  it('should increment count when increment button is clicked', async () => {
    const user = userEvent.setup();
    render(<Counter />);

    const incrementButton = screen.getByRole('button', { name: /increment/i });
    await act(() => user.click(incrementButton)) ;
    
    expect(screen.getByText('Count: 1')).toBeInTheDocument();
  });

  it('should decrement count when decrement button is clicked', async () => {
    const user = userEvent.setup();
    render(<Counter initialCount={5} />);

    const decrementButton = screen.getByRole('button', { name: /decrement/i });
    await act(() =>  user.click(decrementButton)) ;

    expect(screen.getByText('Count: 4')).toBeInTheDocument();
  });

  it('should reset count to 0 when reset button is clicked', async () => {
    const user = userEvent.setup();
    render(<Counter initialCount={10} />);

    const resetButton = screen.getByRole('button', { name: /reset/i });
    await act(() => user.click(resetButton));

    expect(screen.getByText('Count: 0')).toBeInTheDocument();
  });
});