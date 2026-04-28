using GreenHouseApi.Data;
using GreenHouseApi.Models;
using Microsoft.EntityFrameworkCore;

namespace GreenHouseApi.Services;

public class AggregatedMeasurement
{
    public int Index { get; set; }
    public DateTime FirstMeasurementTime { get; set; }
    public DateTime LastMeasurementTime { get; set; }
    public double Min { get; set; }
    public double Average { get; set; }
    public double Max { get; set; }
    public int Count { get; set; }
}

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

    /// <summary>
    /// Returner gennemsnitsværdier af målinger i et tidsperiode
    /// Brug `start` til tidspunkt af første måling der skal medregnes
    /// Brug `secondsPerMeasurement` til at vælge hvor stor en tidsperiode der skal tages gennemsnit over
    /// Brug `count` til at angive hvor mange tidsperioder der skal udregnes
    /// </summary>
    /// <param name="type">Type af måling</param>
    /// <param name="start">Startstidspunkt</param>
    /// <param name="secondsPerMeasurement">Størrelse af tidsperiode til at tage gennemsnit af</param>
    /// <param name="count">Antal tidsperioder</param>
    public async Task<IEnumerable<AggregatedMeasurement>> GetAggregatedMeasurements(string type, DateTime start,
        int secondsPerMeasurement, int count)
    {
        DateTime end = start.AddSeconds(secondsPerMeasurement * count);

        return await db.Measurements
            .Where(t => t.Type == type && t.Timestamp >= start && t.Timestamp < end)
            .GroupBy(t => (int)((t.Timestamp - start).TotalSeconds / secondsPerMeasurement))
            .Select(g => new AggregatedMeasurement
            {
                Index = g.Key,
                Min = g.Min(t => t.Value),
                Max = g.Max(t => t.Value),
                Average = g.Average(t => t.Value),
                Count = g.Count(),
                FirstMeasurementTime = g.Min(t => t.Timestamp),
                LastMeasurementTime = g.Max(t => t.Timestamp)
            })
            .OrderBy(g => g.Index)
            .ToListAsync();
    }

    public IQueryable<Measurement> AsQueryable()
    {
        return db.Measurements.AsQueryable();
    }
}