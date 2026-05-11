using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using GreenHouseApi.Models;
using GreenHouseApi.Services;
using Microsoft.AspNetCore.Authorization;

namespace GreenHouseApi.Controllers;

public class MeasurementDTO
{
    public string Type { get; set; }
    public double Value { get; set; }
}

[ApiController]
[Authorize]
[Route("api/plants/{plantId:int}/measurements")]
public class MeasurementsController(IMeasurementsService measurements) : ControllerBase
{
    [HttpPost]
    public async Task<ActionResult<Measurement>> PostMeasurement([FromRoute] int plantId,
        [FromBody] MeasurementDTO data)
    {
        await measurements.AddMeasurement(new Measurement
            {
                PlantId = plantId,
                Timestamp = DateTime.UtcNow,
                Type = data.Type,
                Value = data.Value
            }
        );

        return CreatedAtAction(nameof(GetLatest), new { }, data);
    }

    // GET api/plants/{plantId:int}/measurements/latest?type=<type> - hent seneste måling
    [HttpGet("latest")]
    public async Task<ActionResult<Measurement>> GetLatest([FromRoute] int plantId, [FromQuery] string? type)
    {
        if (type is not null)
        {
            var latest = await measurements.GetLatest(plantId, type);

            if (latest == null) return NotFound();

            return Ok(latest);
        }

        return Ok(await measurements.GetLatestAll(plantId));
    }

    // GET api/plants/{plantId:int}/measurements?type=<type> - hent alle målinger (med valgfrit tidsfilter)
    [HttpGet]
    public async Task<ActionResult<List<Measurement>>> GetAll(
        [FromRoute] int plantId,
        [FromQuery] string type,
        [FromQuery] DateTime? from,
        [FromQuery] DateTime? to,
        [FromQuery] int limit = 20,
        [FromQuery] int offset = 0)
    {
        var query = measurements.AsQueryable().Where(m => m.PlantId == plantId && m.Type == type);

        if (from.HasValue)
            query = query.Where(s => s.Timestamp >= from.Value);
        if (to.HasValue)
            query = query.Where(s => s.Timestamp <= to.Value);

        return await query
            .OrderByDescending(s => s.Timestamp)
            .Skip(offset)
            .Take(limit)
            .ToListAsync();
    }

    // GET api/plants/{plantId:int}/measurements/aggregate?type=<type> - hent aggregeringer af målinger
    [HttpGet("aggregate")]
    public async Task<ActionResult<IEnumerable<AggregatedMeasurement>>> Aggregate(
        [FromRoute] int plantId,
        [FromQuery] string type,
        [FromQuery] DateTime start,
        [FromQuery] int secondsPerMeasurement,
        [FromQuery] int count = 24)
    {
        return Ok(await measurements.GetAggregatedMeasurements(plantId, type, start, secondsPerMeasurement, count));
    }
}