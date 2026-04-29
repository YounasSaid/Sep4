using System.Net;
using System.Net.Sockets;
using GreenHouseApi.Services;

namespace GreenHouseApi.SocketServer;

public class SocketServer(IServiceScopeFactory scopeFactory) : BackgroundService
{
    const int Port = 23; // Arduino forventer port 23

    /// <summary>
    /// Til logging, implementering kan eventuelt udskiftes med noget andet logging
    /// </summary>
    /// <param name="message">Besked</param>
    private void Log(string message)
    {
        Console.WriteLine("[IoT] " + message);
    }

    private List<IotSocket> _sockets = [];

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        Log($"Starter...");

        IPHostEntry ipHost = await Dns.GetHostEntryAsync(Dns.GetHostName(), stoppingToken);
        IPAddress ipAddr = ipHost.AddressList[0];
        IPEndPoint localEndPoint = new IPEndPoint(ipAddr, Port);

        Log($"Server åbener på {ipAddr}:{Port}");

        Socket listener = new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

        try
        {
            listener.Bind(localEndPoint);

            listener.Listen(10);

            Log("Klar");

            while (true)
            {
                Socket clientSocket = await listener.AcceptAsync(stoppingToken);

                Log("Ny socket forbundet");

                using var scope = scopeFactory.CreateScope();

                var db = scope.ServiceProvider.GetRequiredService<IMeasurementsService>();
                var logger = scope.ServiceProvider.GetRequiredService<ILogger<IotSocket>>();

                var iotsocket = new IotSocket(clientSocket, db, logger);

                _sockets.Add(iotsocket);

                _ = Task.Run(iotsocket.Loop, stoppingToken);
            }
        }

        catch (Exception e)
        {
            Log(e.ToString());
        }
    }
}