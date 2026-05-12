using GreenHouseApi.Models;
using GreenHouseApi.Services;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

namespace GreenHouseApi.Controllers;

public class PlantDTO
{
    public string Name { get; set; }
    public string Type { get; set; }
}

[ApiController]
[Authorize]
[Route("api/plants")]
public class PlantsController(IPlantService ws) : ControllerBase
{
    // POST api/plants - Opret en ny plante, givet en body med navn og type
    [HttpPost]
    public async Task<ActionResult> Create([FromBody] PlantDTO data)
    {
        var plant = await ws.AddPlant(data.Name, data.Type);
        
        return Created("api/plants/" + plant.PlantId, plant);
    }
    
    // GET api/plants - Hent alle planter
    [HttpGet]
    public async Task<ActionResult> GetAll()
    {
        return Ok(await ws.GetPlants());
    }
    
    // GET api/plants/{plantId:int} - Hent en specifik plante
    [HttpGet]
    [Route("{plantId:int}")]
    public async Task<ActionResult> Get([FromRoute] int plantId)
    {
        return Ok(await ws.GetPlant(plantId));
    }
    
    // POST api/plants/{plantId:int} - Opdater en plante, givet en body med navn og type
    [HttpPost]
    [Route("{plantId:int}")]
    public async Task<ActionResult> Edit([FromRoute] int plantId, [FromBody] PlantDTO data)
    {
        await ws.EditPlant(plantId, data.Name, data.Type);
        
        return Ok("Planten er blevet opdateret");
    }
}