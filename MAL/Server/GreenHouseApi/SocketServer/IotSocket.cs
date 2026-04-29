using System.Net.Sockets;
using System.Runtime.InteropServices.JavaScript;
using System.Text;
using GreenHouseApi.Models;
using GreenHouseApi.Services;

namespace GreenHouseApi.SocketServer;

public class IotSocket(Socket socket, IServiceScopeFactory scopeFactory, ILogger<IotSocket> logger)
{
    public async Task Loop()
    {
        byte[] bytes = new Byte[1024];
        string data = "";

        try
        {
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
}