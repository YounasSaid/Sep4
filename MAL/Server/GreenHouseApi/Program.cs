using Microsoft.EntityFrameworkCore;
using GreenHouseApi.Auth;
using GreenHouseApi.Data;
using GreenHouseApi.Services;
using GreenHouseApi.SocketServer;
using Microsoft.AspNetCore.Authentication;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

builder.Services.AddCors(options =>
{
    options.AddDefaultPolicy(policy =>
    {
        policy.WithOrigins(
                "http://localhost:5173", // Udvikler adresse
                "https://localhost:5173", // Udvikler adresse
                "http://4.223.137.178",
                "https://4.223.137.178")
              .AllowAnyMethod()
              .AllowAnyHeader()
              .AllowCredentials(); // KRÆVES for HttpOnly cookies
    });
});

var connectionString = builder.Configuration.GetConnectionString("DefaultConnection")
                       ?? throw new InvalidOperationException(
                           "ConnectionStrings:DefaultConnection er ikke sat. Sæt env var ConnectionStrings__DefaultConnection.");

builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseNpgsql(connectionString));

// IoT socket server
builder.Services.AddHostedService<SocketServer>();

builder.Services.AddScoped<IPlantService, PlantService>();
builder.Services.AddScoped<IMeasurementsService, MeasurementsService>();
builder.Services.AddSingleton<IWateringService, WateringService>();

builder.Services.AddAuthentication("TokenScheme")
    .AddScheme<AuthenticationSchemeOptions, TokenAuthHandler>("TokenScheme", null);

builder.Services.AddAuthorization();

var app = builder.Build();

using (var scope = app.Services.CreateScope())
{
    var services = scope.ServiceProvider;
    try
    {
        var db = services.GetRequiredService<AppDbContext>();
        await db.Database.EnsureCreatedAsync();
    }
    catch (Exception ex)
    {
        var logger = services.GetRequiredService<ILogger<Program>>();
        logger.LogError(ex, "Der skete en fejl under oprettelse af databasen.");
    }
}

app.UseSwagger();
app.UseSwaggerUI();

app.UseCors();

app.UseAuthentication();
app.UseAuthorization();

app.MapGet("/", () => "GreenHouseApi is running");
app.MapControllers();

app.Run();
