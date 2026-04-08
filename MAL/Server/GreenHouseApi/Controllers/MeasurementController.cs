using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using GreenHouseApi.Models;
using GreenHouseApi.Services;

namespace GreenHouseApi.Controllers;

public class MeasurementDTO
{
    public string Type { get; set; }
    public double Value { get; set; }
}

[ApiController]
[Route("api/[controller]")]
public class MeasurementController(IMeasurementsService measurements) : ControllerBase
{
    // POST api/measurements - modtag data fra IoT
    [HttpPost]
    public async Task<ActionResult<Measurement>> PostMeasurement(MeasurementDTO data)
    {
        await measurements.AddMeasurement(new Measurement
            {
                Timestamp = DateTime.UtcNow,
                Type = data.Type,
                Value = data.Value
            }
        );

        return CreatedAtAction(nameof(GetLatest), new { }, data);
    }

    // GET api/measurements/latest?type=<type> - hent seneste måling
    [HttpGet("latest")]
    public async Task<ActionResult<Measurement>> GetLatest([FromQuery] string type)
    {
        var latest = await measurements.GetLatest(type);

        if (latest == null) return NotFound();

        return latest;
    }

    // GET api/measurements?type=<type> - hent alle målinger (med valgfrit tidsfilter)
    [HttpGet]
    public async Task<ActionResult<List<Measurement>>> GetAll(
        [FromQuery] string type,
        [FromQuery] DateTime? from,
        [FromQuery] DateTime? to)
    {
        var query = measurements.AsQueryable().Where(m => m.Type == type);

        if (from.HasValue)
            query = query.Where(s => s.Timestamp >= from.Value);
        if (to.HasValue)
            query = query.Where(s => s.Timestamp <= to.Value);

        return await query.OrderBy(s => s.Timestamp).ToListAsync();
    }
}