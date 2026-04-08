using System.ComponentModel.DataAnnotations;

namespace GreenHouseApi.Models;

public class Plant
{
    [Key] public int PlantId { get; set; }
    public string PlantName { get; set; } = String.Empty;
    public string PlantType { get; set; } = String.Empty;
    public List<Measurement> Measurements { get; set; } = [];
}