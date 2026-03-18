using Microsoft.EntityFrameworkCore;
using GreenHouseApi.Models;

namespace GreenHouseApi.Data;

public class AppDbContext : DbContext
{
    public AppDbContext(DbContextOptions<AppDbContext> options) : base(options) { }

    public DbSet<SensorData> SensorData => Set<SensorData>();
}
