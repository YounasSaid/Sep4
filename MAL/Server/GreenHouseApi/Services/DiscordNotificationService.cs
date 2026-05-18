using System.Text;
using System.Text.Json;

namespace GreenHouseApi.Services;

public class DiscordNotificationService(IConfiguration config) : INotificationService
{
    private static readonly HttpClient HttpClient = new();
    private readonly string? _webhookUrl = config["DiscordWebhookUrl"];

    public void SendNotification(string message)
    {
        if (string.IsNullOrEmpty(_webhookUrl)) return;

        var payload = new
        {
            content = message
        };

        var json = JsonSerializer.Serialize(payload);
        var content = new StringContent(json, Encoding.UTF8, "application/json");

        HttpClient.PostAsync(_webhookUrl, content).GetAwaiter().GetResult();
    }
}

public interface INotificationService
{
    public void SendNotification(string message);
}