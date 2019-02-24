using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace TCPServerArduino
{
    class TCPServer
    {
        private TcpListener server;
        private IPAddress localAddr;
        private Byte[] bytes;
        private string data;
        private NetworkStream stream;
        public TCPServer(string IP, int Port)
        {
            localAddr = IPAddress.Parse(IP);
            server = new TcpListener(localAddr, Port);
            bytes = new byte[4096];
            data = null;
            // thread = new Thread(SendCommand);
        }

        public void Start()
        {
            server.Start();
            Console.WriteLine("Server started");
        }

        public void ReceiveAndPrintData()
        {
            TcpClient client = server.AcceptTcpClient();
            Console.WriteLine("Connected with RPB");
            stream = client.GetStream();


            while (true)
            {

                int i = 0;
                while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
                {
                    data = Encoding.ASCII.GetString(bytes, 0, i);
                    Console.Write(data);
                }
           
            }
        }
        public void StopServer()
        {
            server.Stop();
        }



    }
}
