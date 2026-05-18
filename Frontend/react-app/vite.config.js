import { defineConfig } from "vite";
import path from 'path'
import react from "@vitejs/plugin-react";

export default defineConfig({
  base: "/",
  plugins: [react()], // TypeScript intellisense
  test: {
    globals: true,
    environment: "jsdom",
    setupFiles: "./src/test/setupTests.ts",
  },
  server: {
    host: true, // Listen on all addresses
    port: 5173,
    strictPort: true,
    open: true, // Auto-open browser
    cors: true,
  },
  resolve: {
    // import Button from '@components/Button' 
    // instead of ../../components/Button
    alias: {
      '@': path.resolve(__dirname, './src'),
      '@components': path.resolve(__dirname, './src/components'),
      '@utils': path.resolve(__dirname, './src/utils'),
    }
  }
}); 