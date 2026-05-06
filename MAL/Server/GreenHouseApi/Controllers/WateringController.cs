using GreenHouseApi.Services;
using Microsoft.AspNetCore.Mvc;

namespace GreenHouseApi.Controllers;

[ApiController]
[Route("api/[controller]")]
public class WateringController(IWateringService ws) : ControllerBase
{
    [HttpPost]
    public async Task<ActionResult> WaterPlant([FromQuery] int plantId, [FromBody] double ml)
    {
        ws.WaterPlant(plantId, ml);

        return Ok("Planten blev vandet!");
    }
}