import { renderHook, waitFor } from '@testing-library/react';
import { useFetch } from './useFetch';

describe('useFetch Hook', () => {
  beforeEach(() => {
    global.fetch = vi.fn();
  });

  afterEach(() => {
    vi.restoreAllMocks();
  });

  it.skip('should return loading state initially', () => {
    global.fetch.mockImplementation(() => 
      Promise.resolve({
        ok: true,
        json: async () => ({ data: 'test' }),
      })
    );

    const { result } = renderHook(() => useFetch('https://api.example.com/data'));

    expect(result.current.loading).toBe(true);
    expect(result.current.data).toBe(null);
    expect(result.current.error).toBe(null);
  });

  it.skip('should return data when fetch succeeds', async () => {
    const mockData = { id: 1, title: 'Test Post' };

    global.fetch.mockImplementation(() =>
      Promise.resolve({
        ok: true,
        json: async () => mockData,
      })
    );

    const { result } = renderHook(() => useFetch('https://api.example.com/posts/1'));

    await waitFor(() => expect(result.current.loading).toBe(false));

    expect(result.current.data).toEqual(mockData);
    expect(result.current.error).toBe(null);
  });

  it.skip('should return error when fetch fails', async () => {
    global.fetch.mockImplementation(() =>
      Promise.resolve({
        ok: false,
      })
    );

    const { result } = renderHook(() => useFetch('https://api.example.com/posts/1'));

    await waitFor(() => expect(result.current.loading).toBe(false));

    expect(result.current.data).toBe(null);
    expect(result.current.error).toBe('Network response was not ok');
  });
});