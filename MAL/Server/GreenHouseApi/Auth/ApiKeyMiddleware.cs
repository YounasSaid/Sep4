namespace GreenHouseApi.Auth;

/// <summary>
/// Middleware der kræver en gyldig X-API-Key header på alle requests
/// undtagen health check (/) og swagger UI.
///
/// API nøglen kommer fra konfigurationen (env var ApiKey eller appsettings).
/// </summary>
public class ApiKeyMiddleware
{
    private const string ApiKeyHeader = "X-API-Key";
    private readonly RequestDelegate _next;
    private readonly string _expectedKey;
    private readonly ILogger<ApiKeyMiddleware> _logger;

    public ApiKeyMiddleware(RequestDelegate next, IConfiguration config, ILogger<ApiKeyMiddleware> logger)
    {
        _next = next;
        _logger = logger;
        _expectedKey = config["ApiKey"]
            ?? throw new InvalidOperationException(
                "ApiKey er ikke konfigureret. Sæt env var 'ApiKey' eller tilføj 'ApiKey' i appsettings.");
    }

    public async Task InvokeAsync(HttpContext context)
    {
        // Public endpoints - intet auth nødvendigt
        if (IsPublicPath(context.Request.Path))
        {
            await _next(context);
            return;
        }

        if (!context.Request.Headers.TryGetValue(ApiKeyHeader, out var providedKey)
            || providedKey != _expectedKey)
        {
            _logger.LogWarning("Afvist request til {Path} - manglende eller forkert {Header}",
                context.Request.Path, ApiKeyHeader);
            context.Response.StatusCode = StatusCodes.Status401Unauthorized;
            await context.Response.WriteAsync(
                $"Unauthorized: Manglende eller forkert {ApiKeyHeader} header");
            return;
        }

        await _next(context);
    }

    private static bool IsPublicPath(PathString path)
    {
        // Health check og swagger UI er åbne
        if (path == "/") return true;
        if (path.StartsWithSegments("/swagger")) return true;
        return false;
    }
}
