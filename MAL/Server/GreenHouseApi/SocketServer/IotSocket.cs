using System.Net.Sockets;
using System.Text;
using GreenHouseApi.Models;
using GreenHouseApi.Services;

namespace GreenHouseApi.SocketServer;

public class IotSocket(Socket socket, IServiceScopeFactory scopeFactory, ILogger<IotSocket> logger, string expectedApiKey)
{
    private const string AuthPrefix = "AUTH:";

    public async Task Loop()
    {
        byte[] bytes = new Byte[1024];
        string data = "";

        try
        {
            // Først skal Arduino sende sin auth: "AUTH:<key>;"
            if (!await AuthenticateAsync(bytes))
            {
                return;
            }

            // ReceiveAsync fra auth-buffer kan have læst data ud over auth - vi
            // genbruger derfor ikke buffer, men starter ren herfra
            bytes = new Byte[1024];

            while (socket.Connected)
            {
                int numByte = await socket.ReceiveAsync(bytes);

                if (numByte == 0)
                {
                    logger.LogInformation("Arduino lukkede forbindelsen pænt (0 bytes modtaget).");
                    break;
                }

                data += Encoding.ASCII.GetString(bytes, 0, numByte);

                while (data.IndexOf(';') > -1)
                {
                    int index = data.IndexOf(';');
                    var entry = data.Substring(0, index);
                    data = data.Substring(index + 1);

                    logger.LogWarning("Fik entry ({Entry}) ({DateTime})", entry, DateTime.UtcNow);

                    if (String.IsNullOrWhiteSpace(entry)) continue;

                    string type = entry.Split(",")[0];
                    string val = entry.Split(",")[1];

                    // Håndter fejl hvis type er "error"
                    if (type == "error")
                    {
                        logger.LogWarning("Arduino stød på en fejl: " + val);
                    }
                    else
                    {
                        using var scope = scopeFactory.CreateScope();

                        var measurements = scope.ServiceProvider.GetRequiredService<IMeasurementsService>();

                        await measurements.AddMeasurement(new Measurement
                        {
                            Timestamp = DateTime.UtcNow,
                            Type = type,
                            Value = double.Parse(val)
                        });
                    }
                }

                if (data.Length > 0)
                {
                    logger.LogInformation(
                        "En halv besked var tilovers, bliver medregnet så snart resten af beskeden bliver modtaget... " +
                        data);
                }
            }
        }
        catch (Exception e)
        {
            logger.LogError(e, "Der skete en fejl under forbindelsen til Arduino.");
        }
        finally
        {
            socket.Close();
            socket.Dispose();
            logger.LogCritical("Socket er nu lukket og ressourcer er frigivet.");
        }
    }

    /// <summary>
    /// Læser fra socket indtil ';' modtages og verificerer at indholdet er
    /// "AUTH:&lt;forventet nøgle&gt;". Lukker socket og returnerer false hvis
    /// authentication mislykkes.
    /// </summary>
    private async Task<bool> AuthenticateAsync(byte[] buffer)
    {
        string authData = "";

        while (!authData.Contains(';'))
        {
            int n = await socket.ReceiveAsync(buffer);
            if (n == 0)
            {
                logger.LogWarning("Klient lukkede inden authentication.");
                return false;
            }
            authData += Encoding.ASCII.GetString(buffer, 0, n);

            // Beskyt mod misbrug - hvis nogen sender uendelig data uden ';'
            if (authData.Length > 256)
            {
                logger.LogWarning("Auth besked for lang, lukker forbindelse.");
                return false;
            }
        }

        string authMessage = authData.Substring(0, authData.IndexOf(';'));

        if (!authMessage.StartsWith(AuthPrefix)
            || authMessage.Substring(AuthPrefix.Length) != expectedApiKey)
        {
            logger.LogWarning("Forkert eller manglende API key fra socket klient. Lukker.");
            return false;
        }

        logger.LogInformation("Socket klient authenticeret.");
        return true;
    }
}
