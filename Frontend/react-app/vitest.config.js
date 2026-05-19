import { defineConfig } from 'vitest/config';
import react from '@vitejs/plugin-react';

export default defineConfig({
  plugins: [react()],
  test: {
    globals: true, // exposes 'describe' and 'it' avoid import them
    environment: 'jsdom',
    setupFiles: './src/test/setup.js',
    },
  }); 