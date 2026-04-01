using System.Net.Sockets;
using System.Text;
using GreenHouseApi.Data;
using GreenHouseApi.Models;

namespace GreenHouseApi.SocketServer;

public class IotSocket
{
    public IotSocket(Socket socket, AppDbContext db)
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
                await db.SensorData.AddAsync(new SensorData
                {
                    Timestamp = DateTime.Now,
                    Type = data.Split(",")[0],
                    Value = double.Parse(data.Split(",")[1])
                });

                await db.SaveChangesAsync();
            });
        }
    }
}