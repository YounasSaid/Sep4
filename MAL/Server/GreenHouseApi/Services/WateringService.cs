namespace GreenHouseApi.Services;

public class WateringService : IWateringService
{
    public class WateringListener(int plantId, Action<ushort> callback, Action stop) : IWateringService.IWateringListener
    {
        public readonly int PlantId = plantId;
        public readonly Action<ushort> Callback = callback;

        public void Stop()
        {
            stop();
        }
    }
    
    private readonly List<WateringListener> _listeners = [];
    
    public void WaterPlant(int plantId, ushort ml)
    {
        foreach (WateringListener listener in _listeners)
        {
            if (listener.PlantId == plantId) listener.Callback(ml);
        }
    }

    public IWateringService.IWateringListener ListenForWatering(int plantId, Action<ushort> callback)
    {
        WateringListener? l = null;
        l = new WateringListener(plantId, callback, (() =>
        {
            if (l is null) return;
            lock (_listeners) _listeners.Remove(l);
        }));
        lock (_listeners) _listeners.Add(l);
        return l;
    }
}