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
    ?? "Host=localhost;Port=5432;Database=greenhouse;Username=postgres;Password=postgres";

builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseNpgsql(connectionString));

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

app.MapControllers();
app.Run();
