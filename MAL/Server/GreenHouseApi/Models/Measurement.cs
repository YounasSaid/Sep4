namespace GreenHouseApi.Models;

public class Measurement
{
    public int Id { get; set; }
    public double HeightMm { get; set; }
    public int LeafCount { get; set; }
    public int HealthScore { get; set; }
    public DateTime Timestamp { get; set; }
}
