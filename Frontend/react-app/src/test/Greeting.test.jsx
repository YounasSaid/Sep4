import { afterEach, expect, test } from "vitest"
import { cleanup, render, screen, within } from '@testing-library/react';
import "@testing-library/jest-dom/vitest"

import { Greeting } from './Greeting';

afterEach(() => {
  cleanup()
})

describe('Greeting Component', () => {
  it('should render the greeting with the provided name', () => 
    {
    render(<Greeting name="Alice" />);

    const heading = screen.getByRole('heading', { level: 1 });
    expect(heading).toHaveTextContent('Hello, Alice!');
    });

  it('should render the welcome message', () => 
    {
    render(<Greeting name="Bob" />);

    const paragraph = screen.getByText('Welcome to our application');
    expect(paragraph).toBeInTheDocument();
    });
  });