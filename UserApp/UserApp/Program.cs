using System;
using System.IO.Ports;
using System.Threading;

namespace UserApp
{
    class Program
    {
        static bool _continue;
        static SerialPort _serialPort;

        public static void Main(string[] args)
        {
            string name;
            string message;
            StringComparer stringComparer = StringComparer.OrdinalIgnoreCase;
            Thread readThread = new Thread(Read);
            _serialPort = new SerialPort();

            // Read more about arduinos serial and probably set up parity (there should be no parity bit), and start and stop bits, because it's somewhat shifting the data
            string portName = SetPortName(_serialPort.PortName);
            int baudRate = 9600;
            Parity parity = Parity.None;

            _serialPort.PortName = portName;
            _serialPort.BaudRate = baudRate;
            _serialPort.Parity = parity;

            _serialPort.ReadTimeout = 500;
            _serialPort.WriteTimeout = 500;

            _serialPort.Open();
            _continue = true;
            readThread.Start();

            Console.WriteLine("Name: ");
            name = Console.ReadLine();

            Console.WriteLine("Type QUIT to exit");

            while(_continue)
            {
                message = Console.ReadLine();

                if(stringComparer.Equals("quit", message))
                {
                    _continue = false;
                }
                else
                {
                    _serialPort.WriteLine(message);
                }
            }

            readThread.Join();
            _serialPort.Close();
        }

        public static void Read()
        {
            bool startSequence = false;

            while (_continue)
            {
                try
                {
                    string message = _serialPort.ReadLine();
                    if(message != string.Empty && !startSequence)
                    {
                        Console.WriteLine("Serial: ");
                        startSequence = true;
                    }else if(message == string.Empty && startSequence)
                    {
                        startSequence = false;
                    }
                    Console.WriteLine(message);
                }catch(TimeoutException) { }
            }
        }

        public static string SetPortName(string defaultPortName)
        {
            string portName;

            Console.WriteLine("Available Ports:");
            foreach (string s in SerialPort.GetPortNames())
            {
                Console.WriteLine("   {0}", s);
            }

            Console.Write("Enter COM port value (Default: {0}): ", defaultPortName);
            portName = Console.ReadLine();

            if (portName == "" || !(portName.ToLower()).StartsWith("com"))
            {
                portName = defaultPortName;
            }
            return portName;
        }
    }
}