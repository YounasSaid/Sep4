using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using GreenHouseApi.Data;
using GreenHouseApi.Models;

namespace GreenHouseApi.Controllers;

[ApiController]
[Route("api/[controller]")]
public class SensorDataController : ControllerBase
{
    private readonly AppDbContext _context;

    public SensorDataController(AppDbContext context)
    {
        _context = context;
    }

    // POST api/sensordata - modtag data fra IoT
    [HttpPost]
    public async Task<ActionResult<SensorData>> PostSensorData(SensorData data)
    {
        data.Timestamp = DateTime.UtcNow;
        _context.SensorData.Add(data);
        await _context.SaveChangesAsync();
        return CreatedAtAction(nameof(GetLatest), new { }, data);
    }

    // GET api/sensordata/latest - hent seneste måling
    [HttpGet("latest")]
    public async Task<ActionResult<SensorData>> GetLatest()
    {
        var latest = await _context.SensorData
            .OrderByDescending(s => s.Timestamp)
            .FirstOrDefaultAsync();

        if (latest == null) return NotFound();
        return latest;
    }

    // GET api/sensordata - hent alle målinger (med valgfrit tidsfilter)
    [HttpGet]
    public async Task<ActionResult<List<SensorData>>> GetAll(
        [FromQuery] DateTime? from,
        [FromQuery] DateTime? to)
    {
        var query = _context.SensorData.AsQueryable();

        if (from.HasValue)
            query = query.Where(s => s.Timestamp >= from.Value);
        if (to.HasValue)
            query = query.Where(s => s.Timestamp <= to.Value);

        return await query.OrderBy(s => s.Timestamp).ToListAsync();
    }
}
