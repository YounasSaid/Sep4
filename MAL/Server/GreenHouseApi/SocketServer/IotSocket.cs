using System.Net.Sockets;
using System.Text;
using GreenHouseApi.Models;
using GreenHouseApi.Services;

namespace GreenHouseApi.SocketServer;

public class IotSocket(
    Socket socket,
    IWateringService ws,
    IServiceScopeFactory scopeFactory,
    ILogger<IotSocket> logger,
    string expectedApiKey)
{
    private const string AuthPrefix = "auth,";

    private IWateringService.IWateringListener? _wateringListener;

    private int _plantId = 0;

    public async Task Loop()
    {
        byte[] bytes = new Byte[1024];
        string data = "";

        try
        {
            // Først skal Arduino sende sin auth: "auth:<key>;"
            if (!await AuthenticateAsync(bytes))
            {
                return;
            }
            
            // Begynd periodisk at pinge arduinoen, da den ikke selv opdager når forbindelsen bliver tabt...

            _ = PeriodicPing();

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

                    if (String.IsNullOrWhiteSpace(entry)) continue;

                    string type = entry.Split(",")[0];
                    string val = entry.Split(",")[1];

                    // Håndter fejl hvis type er "error"
                    if (type == "error")
                    {
                        logger.LogWarning("Arduino stød på en fejl: {val}", val);
                    }
                    else if (type == "id")
                    {
                        using var scope = scopeFactory.CreateScope();

                        var plants = scope.ServiceProvider.GetRequiredService<IPlantService>();

                        int id = int.Parse(val);

                        if (await plants.GetPlant(id) == null)
                        {
                            logger.LogWarning(
                                "Arduino har skiftet til en ikke-eksisterende plante id: {id}, en ukendt plante er derfor blevet oprettet",
                                id);
                            await plants.AddPlant(id, "Ukendt plante", "ukendt");
                        }
                        else
                        {
                            logger.LogWarning("En arduino har skiftet til plante id: {id}", id);
                        }

                        _plantId = id;

                        if (_wateringListener is not null) _wateringListener.Stop();

                        // Indstil water listener
                        _wateringListener = ws.ListenForWatering(_plantId, async ml =>
                        {
                            var payload = Encoding.ASCII.GetBytes($"water,{ml};");
                            if (socket.Connected) await socket.SendAsync(payload);
                        });
                    }
                    else
                    {
                        using var scope = scopeFactory.CreateScope();

                        var measurements = scope.ServiceProvider.GetRequiredService<IMeasurementsService>();

                        await measurements.AddMeasurement(new Measurement
                        {
                            Timestamp = DateTime.UtcNow,
                            PlantId = _plantId,
                            Type = type,
                            Value = double.Parse(val)
                        });
                    }
                }

                if (data.Length > 0)
                {
                    logger.LogInformation(
                        "En halv besked var tilovers, bliver medregnet så snart resten af beskeden bliver modtaget... {data}",
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
            try
            {
                _wateringListener?.Stop();
            }
            catch (Exception e)
            {
                logger.LogWarning(e, "Kunne ikke afregistrere watering listener korrekt.");
            }

            socket.Close();
            socket.Dispose();
            logger.LogCritical("Socket er nu lukket og ressourcer er frigivet.");
        }
    }

    private async Task PeriodicPing()
    {
        using var timer = new PeriodicTimer(TimeSpan.FromSeconds(30));

        while (await timer.WaitForNextTickAsync())
        {
            if (!socket.Connected) break;
            await socket.SendAsync("ping,30;"u8.ToArray());
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

        string authMessage = authData[..authData.IndexOf(';')];

        if (!authMessage.StartsWith(AuthPrefix) || authMessage[AuthPrefix.Length..] != expectedApiKey)
        {
            logger.LogWarning("Forkert eller manglende API key fra socket klient. Lukker.");
            return false;
        }

        logger.LogInformation("Socket klient authenticeret.");
        return true;
    }
}