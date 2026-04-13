using System.Net.Sockets;
using System.Text;
using GreenHouseApi.Data;
using GreenHouseApi.Models;
using GreenHouseApi.Services;

namespace GreenHouseApi.SocketServer;

public class IotSocket
{
    public IotSocket(Socket socket, IMeasurementsService measurements)
    {
        byte[] bytes = new Byte[1024];
        string data;

        while (true)
        {
            bytes = new Byte[1024];
            data = null;

            while (true)
            {
                int numByte = socket.Receive(bytes);

                data += Encoding.ASCII.GetString(bytes, 0, numByte);

                if (data.IndexOf(';') > -1) break;
            }

            Console.WriteLine("Socket modtog " + data);

            Task.Run(async () =>
            {
                string type = data.Split(",")[0];
                string val = data.Split(",")[1];

                // Håndter fejl hvis type er "error"
                if (type == "error")
                {
                    Console.WriteLine("[IoT Socket] Arduino stød på en fejl: " + val);
                }
                else
                {
                    await measurements.AddMeasurement(new Measurement
                    {
                        Timestamp = DateTime.Now,
                        Type = type,
                        Value = double.Parse(val)
                    });
                }
            });
        }
    }
}