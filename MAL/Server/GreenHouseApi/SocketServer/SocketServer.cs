using System.Net;
using System.Net.Sockets;
using GreenHouseApi.Services;

namespace GreenHouseApi.SocketServer;

public class SocketServer(IServiceScopeFactory scopeFactory, ILogger<SocketServer> logger, IConfiguration config) : BackgroundService
{
    const int Port = 23; // Arduino forventer port 23

    private readonly string _expectedKey = config["ApiKey"]
        ?? throw new InvalidOperationException(
            "ApiKey er ikke konfigureret. Sæt env var 'ApiKey' eller tilføj 'ApiKey' i appsettings.");

    private List<IotSocket> _sockets = [];

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        logger.LogInformation("Starter...");

        IPHostEntry ipHost = await Dns.GetHostEntryAsync(Dns.GetHostName(), stoppingToken);
        IPAddress ipAddr = ipHost.AddressList[0];
        IPEndPoint localEndPoint = new IPEndPoint(ipAddr, Port);

        logger.LogInformation("Server åbener på {IpAddress}:{Port}", ipAddr, Port);

        Socket listener = new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

        try
        {
            listener.Bind(localEndPoint);

            listener.Listen(10);

            logger.LogInformation("Klar");

            while (true)
            {
                Socket clientSocket = await listener.AcceptAsync(stoppingToken);

                logger.LogInformation("Ny socket forbundet");

                using var scope = scopeFactory.CreateScope();

                var socketLogger = scope.ServiceProvider.GetRequiredService<ILogger<IotSocket>>();

                var iotsocket = new IotSocket(clientSocket, scopeFactory, socketLogger, _expectedKey);

                _sockets.Add(iotsocket);

                _ = Task.Run(iotsocket.Loop, stoppingToken);
            }
        }

        catch (Exception e)
        {
            logger.LogInformation(e.ToString());
        }
    }
}
