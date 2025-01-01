
using System;
using System.IO.Ports;

namespace ArduinoPortApp
{
    public static class SerialPortHelper
    {
        public static string[] GetAvailablePorts()
        {
            return SerialPort.GetPortNames(); 
        }
    }
}
