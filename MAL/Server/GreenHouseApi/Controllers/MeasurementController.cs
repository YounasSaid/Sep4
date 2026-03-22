using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using GreenHouseApi.Data;
using GreenHouseApi.Models;

namespace GreenHouseApi.Controllers;

[ApiController]
[Route("api/[controller]")]
public class MeasurementController : ControllerBase
{
    private readonly AppDbContext _context;

    public MeasurementController(AppDbContext context)
    {
        _context = context;
    }

    // POST api/measurement - frontend sender manuelle målinger
    [HttpPost]
    public async Task<ActionResult<Measurement>> PostMeasurement(Measurement data)
    {
        data.Timestamp = DateTime.UtcNow;
        _context.Measurements.Add(data);
        await _context.SaveChangesAsync();
        return CreatedAtAction(nameof(GetAll), new { }, data);
    }

    // GET api/measurement - hent alle manuelle målinger
    [HttpGet]
    public async Task<ActionResult<List<Measurement>>> GetAll(
        [FromQuery] DateTime? from,
        [FromQuery] DateTime? to)
    {
        var query = _context.Measurements.AsQueryable();

        if (from.HasValue)
            query = query.Where(m => m.Timestamp >= from.Value);
        if (to.HasValue)
            query = query.Where(m => m.Timestamp <= to.Value);

        return await query.OrderBy(m => m.Timestamp).ToListAsync();
    }
}
