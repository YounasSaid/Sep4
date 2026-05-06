namespace GreenHouseApi.Services;

public interface IWateringService
{
    public interface IWateringListener
    {
        public void Stop();
    }
    
    public void WaterPlant(int plantId, double ml);
    public IWateringListener ListenForWatering(int plantId, Action<double> water);
}