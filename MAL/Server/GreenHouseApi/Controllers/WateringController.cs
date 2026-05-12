using GreenHouseApi.Services;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

namespace GreenHouseApi.Controllers;

[ApiController]
[Authorize]
[Route("api")]
public class WateringController(IWateringService ws) : ControllerBase
{
    [HttpPost]
    [Route("plants/{plantId:int}/water")]
    public async Task<ActionResult> WaterPlant([FromRoute] int plantId, [FromBody] ushort ml)
    {
        ws.WaterPlant(plantId, ml);

        return Ok("Planten blev vandet!");
    }
}