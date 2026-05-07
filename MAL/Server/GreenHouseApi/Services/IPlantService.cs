using GreenHouseApi.Models;

namespace GreenHouseApi.Services;

public interface IPlantService
{
    Task<Plant> AddPlant(string name, string type);
    Task<Plant> AddPlant(int id, string name, string type);
    Task<Plant?> GetPlant(int plantId);
    Task<IEnumerable<Plant>> GetPlants();
    Task EditPlant(int plantId, string name, string type);
}