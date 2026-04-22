using GreenHouseApi.Data;
using GreenHouseApi.Models;
using Microsoft.EntityFrameworkCore;

namespace GreenHouseApi.Services;

public class MeasurementsService(AppDbContext db) : IMeasurementsService
{
    public async Task AddMeasurement(Measurement measurement)
    {
        await db.Measurements.AddAsync(measurement);
        await db.SaveChangesAsync();
    }

    public async Task<Measurement?> GetLatest(string type)
    {
        return await db.Measurements
            .Where(t => t.Type == type)
            .OrderByDescending(t => t.Timestamp)
            .FirstOrDefaultAsync();
    }

    public IQueryable<Measurement> AsQueryable()
    {
        return db.Measurements.AsQueryable();
    }

    public async Task<List<Measurement>> GetAll()
    {
        return await db.Measurements.ToListAsync();
    }
}
