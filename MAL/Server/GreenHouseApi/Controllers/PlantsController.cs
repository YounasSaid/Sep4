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
public class PlantsController(IPlantService ws, IConfiguration config) : ControllerBase
{
    private string _apiKey = config["ApiKey"] ??
                             throw new InvalidOperationException(
                                 "ApiKey er ikke konfigureret. Sæt env var 'API_KEY' eller tilføj 'ApiKey' i appsettings.");
    private static readonly HttpClient Client = new();

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
    
    
    // POST api/plants/{plantId:int}/evaluate - Hent evaluering fra plante som kalder endpoints på ML-serveren
    // Modellen trænes på ny hver gang dette endpoint kaldes
    [HttpPost]
    [Route("{plantId:int}/evaluate")]
    public async Task<ActionResult> Evaluate([FromRoute] int plantId)
    {
        using var trainRequest = new HttpRequestMessage(HttpMethod.Post, $"http://ml:5001/api/plant/train");
        trainRequest.Headers.Add("X-API-Key", _apiKey);
        
        var trainResponse = await Client.SendAsync(trainRequest);
    
        if (!trainResponse.IsSuccessStatusCode)
        {
            var errorBody = await trainResponse.Content.ReadAsStringAsync();
            return StatusCode((int)trainResponse.StatusCode, $"Træning fejlede: {errorBody}");        }
        
        
        using var evaluateRequest = new HttpRequestMessage(HttpMethod.Post, $"http://ml:5001/api/plant/evaluate?plantId={plantId}");
        evaluateRequest.Headers.Add("X-API-Key", _apiKey);

        var evaluateResponse = await Client.SendAsync(evaluateRequest);

        
        if (!evaluateResponse.IsSuccessStatusCode)
        {
            var errorBody = await evaluateResponse.Content.ReadAsStringAsync();
            return StatusCode((int)evaluateResponse.StatusCode, $"Evaluering fejlede: {errorBody}");        }

        var jsonResult = await evaluateResponse.Content.ReadAsStringAsync();
        
        return Content(jsonResult, "application/json");    }
}