using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TCPServerArduino
{
    class Program
    {
        static void Main(string[] args)
        {
            TCPServer server = new TCPServer("192.168.43.191", 1410);
           
            try
            {
                server.Start();
                server.ReceiveAndPrintData();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally{
                server.StopServer();
            }
        }
    }
}
