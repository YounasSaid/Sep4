using GreenHouseApi.Models;

namespace GreenHouseApi.Services;

public interface IMeasurementsService
{
    Task AddMeasurement(Measurement measurement);
    Task<Measurement?> GetLatest(int plantId, string type);
    Task<AllMeasurements> GetLatestAll(int plantId);
    IQueryable<Measurement> AsQueryable();
    Task<IEnumerable<AggregatedMeasurement>> GetAggregatedMeasurements(int plantId, string type, DateTime start, int secondsPerMeasurement, int count);
}