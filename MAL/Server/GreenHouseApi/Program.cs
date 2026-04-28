using Microsoft.EntityFrameworkCore;
using GreenHouseApi.Data;
using GreenHouseApi.Services;
using GreenHouseApi.SocketServer;

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

var connectionString = builder.Configuration.GetConnectionString("DefaultConnection");

builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseNpgsql(connectionString));

Console.WriteLine("bruger connection string " + connectionString);

// IoT socket server
builder.Services.AddHostedService<SocketServer>();

builder.Services.AddScoped<IPlantService, PlantService>();
builder.Services.AddScoped<IMeasurementsService, MeasurementsService>();

var app = builder.Build();

app.UseSwagger();
app.UseSwaggerUI();

app.UseCors();

app.MapGet("/", () => "GreenHouseApi is running");

app.MapGet("/api/init-db", async (AppDbContext db) =>
{
    try
    {
        await db.Database.EnsureCreatedAsync();
        return Results.Ok("Database tables created");
    }
    catch (Exception ex)
    {
        return Results.Ok($"Error: {ex.Message}");
    }
});

app.MapPost("/api/reset-db", async (AppDbContext db) =>
{
    try
    {
        await db.Database.EnsureDeletedAsync();
        await db.Database.EnsureCreatedAsync();
        return Results.Ok("Database reset and tables recreated");
    }
    catch (Exception ex)
    {
        return Results.Ok($"Error: {ex.Message}");
    }
});

app.MapControllers();

app.Run();