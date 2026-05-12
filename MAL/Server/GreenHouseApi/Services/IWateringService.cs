namespace GreenHouseApi.Services;

public interface IWateringService
{
    public interface IWateringListener
    {
        public void Stop();
    }
    
    public void WaterPlant(int plantId, ushort ml);
    public IWateringListener ListenForWatering(int plantId, Action<ushort> water);
}