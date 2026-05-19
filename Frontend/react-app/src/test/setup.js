import '@testing-library/jest-dom';
import { afterEach } from 'vitest';
import { cleanup } from '@testing-library/react';


afterEach(() => {
  // The cleanup() function runs after each test to clean up the DOM, 
  // ensuring tests don't interfere with each other.
  cleanup();
  });