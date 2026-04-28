using GreenHouseApi.Models;

namespace GreenHouseApi.Services;

public interface IMeasurementsService
{
    Task AddMeasurement(Measurement measurement);
    Task<Measurement?> GetLatest(string type);
    IQueryable<Measurement> AsQueryable();
    Task<IEnumerable<AggregatedMeasurement>> GetAggregatedMeasurements(string type, DateTime start, int secondsPerMeasurement, int count);
}