using Microsoft.AspNetCore.Mvc;

namespace GreenHouseApi.Controllers;

public class LoginDTO
{
    public string Username { get; set; }
    public string Password { get; set; }
}

[ApiController]
[Route("api")]
public class AuthController(IConfiguration config) : ControllerBase
{
    private string _apiKey = config["ApiKey"] ??
                             throw new InvalidOperationException(
                                 "ApiKey er ikke konfigureret. Sæt env var 'API_KEY' eller tilføj 'ApiKey' i appsettings.");

    private string _username = config["WebUsername"] ??
                               throw new InvalidOperationException(
                                   "WebUsername er ikke konfigureret. Sæt env var 'WEB_USERNAME' eller tilføj 'WebUsername' i appsettings.");

    private string _password = config["WebPassword"] ??
                               throw new InvalidOperationException(
                                   "WebPassword er ikke konfigureret. Sæt env var 'WEB_PASSWORD' eller tilføj 'WebPassword' i appsettings.");

    /// <summary>
    /// Kald dette endpoint for at logge ind, i body skal der være "username" og "password"
    /// Dette er p.t. opsat som et enkel-bruger-system hvor brugernavn og adgangskode er opsat i .env.
    /// </summary>
    [HttpPost]
    [Route("login")]
    public IActionResult Login([FromBody] LoginDTO data)
    {
        if (data.Username != _username || !BCrypt.Net.BCrypt.Verify(data.Password, _password))
            return Unauthorized("Forkert brugernavn eller adgangskode");

        return Ok(_apiKey);
    }

    /// <summary>
    /// Kald dette endpoint for at logge ud
    /// </summary>
    [HttpPost]
    [Route("logout")]
    public IActionResult Logout()
    {
        return Ok("Logget ud... (er ikke implementeret)");
    }
}