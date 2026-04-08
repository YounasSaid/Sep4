using System.ComponentModel.DataAnnotations;

namespace GreenHouseApi.Models;

public class Measurement
{
    [Key] public int Id { get; set; }
    public Plant? Plant { get; set; }
    public int? PlantId { get; set; }
    public required string Type { get; set; } = string.Empty;
    public required double Value { get; set; }
    public required DateTime Timestamp { get; set; }
}