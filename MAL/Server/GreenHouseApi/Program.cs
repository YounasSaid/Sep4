using Microsoft.EntityFrameworkCore;
using GreenHouseApi.Data;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

builder.Services.AddCors(options =>
{
    options.AddDefaultPolicy(policy =>
    {
        policy.AllowAnyOrigin()
              .AllowAnyMethod()
              .AllowAnyHeader();
    });
});

var connectionString = builder.Configuration.GetConnectionString("DefaultConnection")
    ?? Environment.GetEnvironmentVariable("ConnectionStrings__DefaultConnection")
    ?? "Host=localhost;Port=5432;Database=greenhouse;Username=postgres;Password=postgres";

builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseNpgsql(connectionString));

var app = builder.Build();

// Auto-opret database tabeller i baggrunden
_ = Task.Run(async () =>
{
    await Task.Delay(5000);
    try
    {
        using var scope = app.Services.CreateScope();
        var db = scope.ServiceProvider.GetRequiredService<AppDbContext>();
        await db.Database.EnsureCreatedAsync();
        app.Logger.LogInformation("Database oprettet/forbundet.");
    }
    catch (Exception ex)
    {
        app.Logger.LogError(ex, "Database fejl ved opstart.");
    }
});

app.UseSwagger();
app.UseSwaggerUI();

app.UseCors();
app.MapControllers();
app.Run();
