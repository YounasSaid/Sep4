using System.Security.Claims;
using System.Text.Encodings.Web;
using Microsoft.AspNetCore.Authentication;
using Microsoft.Extensions.Options;

namespace GreenHouseApi.Auth;

public class TokenAuthHandler(
    IConfiguration config,
    IOptionsMonitor<AuthenticationSchemeOptions> options,
    ILoggerFactory logger,
    UrlEncoder encoder)
    : AuthenticationHandler<AuthenticationSchemeOptions>(options, logger, encoder)
{
    private string _apiKey = config["ApiKey"] ??
                             throw new InvalidOperationException(
                                 "ApiKey er ikke konfigureret. Sæt env var 'API_KEY' eller tilføj 'ApiKey' i appsettings.");
    
    protected override async Task<AuthenticateResult> HandleAuthenticateAsync()
    {
        if (!Request.Cookies.TryGetValue("X-API-Key", out var token))
        {
            if (!Request.Headers.TryGetValue("Authorization", out var authHeader))
                return AuthenticateResult.Fail("Ingen token fundet");

            token = authHeader.ToString().Replace("Bearer ", "");
        }

        if (token != _apiKey)
            return AuthenticateResult.Fail("Ugyldig eller revoked token");

        var identity = new ClaimsIdentity(authenticationType: Scheme.Name);
        var principal = new ClaimsPrincipal(identity);
        var ticket = new AuthenticationTicket(principal, Scheme.Name);

        return AuthenticateResult.Success(ticket);
    }
}