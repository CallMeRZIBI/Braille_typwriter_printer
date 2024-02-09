using System;
using System.IO.Ports;
using System.Text;
using System.Runtime.CompilerServices;
using System.Threading;

namespace Braille_typewriter_frontend
{
    public partial class Form1 : Form
    {
        static Form1 form;

        // Serial things
        private static bool _firstPortSetup;
        private static bool _endSession;
        private static SerialPort _serialPort;
        private static Thread _readThread;
        private static Thread _writeThread;
        private static StringComparer _stringComparer = StringComparer.OrdinalIgnoreCase;
        private static string _portName;
        private static int _baudrate;
        private static bool _recieved;
        private static bool _loadedPaper;
        public static string IncomeMsg;
        public static string SendMsg;

        public Form1()
        {
            InitializeComponent();

            // Subscribing to form closing event
            FormClosing += new FormClosingEventHandler(Form1_FormClosing);

            form = this;

            // Creating a thread for reading from Serial Port
            _readThread = new Thread(Read);
            _writeThread = new Thread(Write);

            // Creating a new Serial Port with default settings
            _serialPort = new SerialPort();

            // User changable setting for serial port
            updatePortNamesDropDown();
            _baudrate = int.Parse(baudRateValue.Text);
            _portName = _serialPort.PortName;

            Encoding.RegisterProvider(System.Text.CodePagesEncodingProvider.Instance);

            // Applying setting fro serial port
            _serialPort.BaudRate = _baudrate;
            _serialPort.Parity = Parity.None;
            _serialPort.Encoding = Encoding.GetEncoding(28592);
            _serialPort.DataBits = 8;
            _serialPort.StopBits = StopBits.One;
            _serialPort.Handshake = Handshake.None;

            // Set read/write timeouts
            _serialPort.ReadTimeout = 500;
            _serialPort.WriteTimeout = 500;

            _recieved = false;
            _endSession = false;
            _firstPortSetup = true;
            _loadedPaper = false;

            // Adding scrollbar to serial communication texbox
            SerialCommunication.ScrollBars = ScrollBars.Vertical;
            SerialCommunication.WordWrap = false;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            _endSession = true;
            while (_readThread.IsAlive)
            {
                SerialCommunication.Text = "Ending tasks";
            }
            if (_readThread.IsAlive)
            {
                _readThread.Interrupt();
                _readThread.Join();
            }
            _serialPort.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // Starting writing to Serial Port process only if previous printing has been completed
            if (!_writeThread.IsAlive)
            {
                _writeThread = new Thread(Write);
                _writeThread.IsBackground = true;
                _writeThread.Start();
            }
            else
            {
                ShowDialogBox("Printer is still printing.");
            }
        }

        private static void Write()
        {
            string message = SendMsg;

            if (message == null) return;

            // Splitting string into chunks
            List<string> payloads = createChunks(message, 15);

            // Starting the printing by letting the user insert paper
            // Sending the same bytes after the printing is done to remove the paper and reset data on arduino
            byte[] start_stop = Encoding.UTF8.GetBytes("\\\r\n");
            _serialPort.Write(start_stop, 0, start_stop.Length);

            while(!_loadedPaper && !_endSession)
            {
                // Waiting for user to insert paper
            }

            for (int i = 0; i < payloads.Count; i++)
            {
                // Writing to Serial Port
                try
                {
                    byte[] byte_payload = Encoding.GetEncoding(28592).GetBytes(payloads[i]);
                    _serialPort.Write(byte_payload, 0, byte_payload.Length);
                    _serialPort.Write("\n");
                    //_serialPort.WriteLine(payloads[i]);
                }
                catch (Exception ex)
                {
                    ShowDialogBox(ex.Message);
                    return;
                }

                while (!_recieved && !_endSession)
                {
                    // Waiting for arduino to stop writing
                }

                // Setting _recieved back to false after recieving message
                _recieved = false;
            }

            // End of printing session character, I have to send it in bytes because otherwise it just probably adds
            // something to it and the arduino just reads something different
            _serialPort.Write(start_stop, 0, start_stop.Length);

            _loadedPaper = false;

            return;
        }

        private static List<string> createChunks(string message, int chunkSize)
        {
            List<string> msgParts = new List<string>();

            // Cannot split one word into chunks or the total count of characters is less or equal chunk size
            if ((!message.Contains(' ') && message.Length <= chunkSize) || message.Length <= chunkSize)
            {
                msgParts.Add(message);
                return msgParts;
            }

            // Splitting message into individual words and appending chunks into msgParts list
            string[] words = message.Split(' ');
            string chunk = "";
            for (int i = 0; i < words.Length; i++)
            {
                // Split the word if it's longer than chunkSize + 10
                if (words[i].Length > chunkSize + 10)
                {
                    int subIndex = 1;
                    while (subIndex * chunkSize <= words[i].Length)
                    {
                        chunk += words[i].Substring((subIndex - 1) * chunkSize, (subIndex * chunkSize) > words[i].Length ? words[i].Length : chunkSize);
                        msgParts.Add(chunk);
                        subIndex++;
                        chunk = "";
                    }
                    continue;
                }

                // Otherwise make chunks conatining words with max chunk length of chunkSize
                chunk += words[i] + " ";
                if (chunk.Length >= chunkSize || i == words.Length - 1)
                {
                    msgParts.Add(chunk);
                    chunk = "";
                }
            }

            return msgParts;
        }

        private static void Read()
        {
            while (!_endSession)
            {
                IncomeMsg = string.Empty;

                try
                {
                    IncomeMsg = _serialPort.ReadLine();
                }
                catch (TimeoutException) { }

                // Setting _recieved to true when done message arrived => the printing process for the chunk is completed
                if (IncomeMsg.Contains("done"))
                {
                    _recieved = true;
                }
                else if (IncomeMsg.Contains("loaded paper")) 
                {
                    _loadedPaper = true;
                }

                if (IncomeMsg != string.Empty && !_endSession)
                {
                    // Invoking (changing threads), because you cannot change form variables from different thread
                    form.SerialCommunication.Invoke((MethodInvoker)delegate
                    {
                        form.SerialCommunication.AppendText(IncomeMsg + "\r\n");
                    });
                }
            }
        }

        private void updatePortNamesDropDown()
        {
            portNameDropDown.Items.Clear();

            foreach (string s in SerialPort.GetPortNames())
            {
                portNameDropDown.Items.Add(s);
            }
        }

        private void portNameDropDown_Click(object sender, EventArgs e)
        {
            updatePortNamesDropDown();
        }

        private void portNameDropDown_SelectedIndexChanged(object sender, EventArgs e)
        {
            string defaultPortName = _serialPort.PortName;
            string portName;

            portName = portNameDropDown.SelectedItem as string;
            if (portName == string.Empty || !(portName.ToLower()).StartsWith("com"))
            {
                portName = defaultPortName;
            }

            _portName = portName;
        }

        private void baudRateValue_TextChanged(object sender, EventArgs e)
        {
            _baudrate = int.Parse(baudRateValue.Text);
        }

        private void userPaylaod_TextChanged(object sender, EventArgs e)
        {
            SendMsg = userPaylaod.Text;
        }

        private void apply_Click(object sender, EventArgs e)
        {
            // Close serial port if it was already opened
            _endSession = true; // Stop Threads from inside loop
            if (!_firstPortSetup)
            {
                while (_readThread.IsAlive)
                {
                    // Waiting until thread stops
                }

                _serialPort.Close();
                _readThread.Interrupt();
                _readThread.Join();
                if (_writeThread.IsAlive)
                {
                    _writeThread.Join();
                }
            }
            _endSession = false;    // Enable inside loops of threads

            // Complete the setup, it's only possible after getting the port name
            _serialPort.PortName = _portName;
            _serialPort.BaudRate = _baudrate;

            try
            {
                _serialPort.Open();
                // The Port was set up
                _firstPortSetup = false;
            }
            catch (Exception ex)
            {
                ShowDialogBox(ex.Message);
                return;
            }

            // Creating the thread, and because I don't know if it's the first time or not
            // and I don't really care if the thread already exists, I'll just create a new one
            _readThread = new Thread(Read);
            _readThread.Start();
        }

        private static void ShowDialogBox(string text)
        {
            var formPopup = new Popup();
            formPopup.DisplayText(text);
            formPopup.ShowDialog();
        }
    }
}