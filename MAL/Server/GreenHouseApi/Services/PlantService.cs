using GreenHouseApi.Data;
using GreenHouseApi.Models;
using Microsoft.EntityFrameworkCore;

namespace GreenHouseApi.Services;

public class PlantService(AppDbContext db) : IPlantService
{
    public async Task<Plant> AddPlant(string name, string type)
    {
        var res = await db.Plants.AddAsync(new Plant
        {
            PlantName = name,
            PlantType = type
        });
        await db.SaveChangesAsync();
        return res.Entity;
    }

    public async Task<Plant?> GetPlant(int plantId)
    {
        return await db.Plants.FindAsync(plantId);
    }

    public async Task<IEnumerable<Plant>> GetPlants()
    {
        return await db.Plants.ToListAsync();
    }

    public async Task EditPlant(int plantId, string name, string type)
    {
        var plant = await GetPlant(plantId);
        if (plant == null) throw new Exception("Plant not found");
        plant.PlantName = name;
        plant.PlantType = type;
        await db.SaveChangesAsync();
    }
}