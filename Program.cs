using System;
using OpenHardwareMonitor.Hardware;
using System.IO.Ports;
using System.Threading;
using Sharer;
using System.Collections.Generic;


namespace PPSU_hwmonitor_c
{
    class Program
    {
        /**
         *  Define vars to hold stats
         **/

        // CPU Temp
        static float cpuTemp;
        // CPU Usage
        static float cpuUsage;
        // CPU Power Draw (Package)
        static float cpuPowerDrawPackage;
        // CPU Frequency
        static float cpuFrequency;
        // GPU Temperature
        static float gpuTemp;
        // GPU Usage
        static float gpuUsage;
        // GPU Core Frequency
        static float gpuCoreFrequency;
        // GPU Memory Frequency
        static float gpuMemoryFrequency;

        // create a new SerialPort object
        //SerialPort serialPort = new SerialPort("COM5", 9600);



        /**
         * Init OpenHardwareMonitor.dll Computer Object
         **/

        static Computer c = new Computer()
        {
            GPUEnabled = true,
            CPUEnabled = true,
            //RAMEnabled = true, // uncomment for RAM reports
            //MainboardEnabled = true, // uncomment for Motherboard reports
            //FanControllerEnabled = true, // uncomment for FAN Reports
            //HDDEnabled = true, // uncomment for HDD Report
        };

        /**
         * Pulls data from OHM
         **/

        static String ReportSystemInfo()
        {   //set which values to send to serial monitor
            //Must set less than or equal to 4 values as true
            bool GPUTemp = false; //**testing computer does not have independent GPU
            bool GPUUsage = false;
            bool GPUFrequency = false;
            bool GPUMemory = false;

            bool CPUTemp = true;
            bool CPUUsage = true;
            bool CPUPackage = true;
            bool CPUFrequency = true;

            int setvals = GPUTemp ? 1 : 0;
            setvals += GPUUsage ? 1 : 0;
            setvals += GPUFrequency ? 1 : 0;
            setvals += GPUMemory ? 1 : 0;
            setvals += CPUTemp ? 1 : 0;
            setvals += CPUUsage ? 1 : 0;
            setvals += CPUPackage ? 1 : 0;
            setvals += CPUFrequency ? 1 : 0;
            System.Diagnostics.Debug.WriteLine("setvals: " + setvals);
            if(setvals > 4)
            {
                return "#More than 4 values were set to be viewed. Please keep them below or equal to 4$#$#$#$";
            }

            String data = ""; //will hold serial data
            foreach (var hardware in c.Hardware)
            {

                if (hardware.HardwareType == HardwareType.CPU)
                {
                    // only fire the update when found
                    hardware.Update();

                    // loop through the data
                    foreach (var sensor in hardware.Sensors)
                        if (sensor.SensorType == SensorType.Temperature && sensor.Name.Contains("CPU Package") && CPUTemp)
                        {
                            // store
                            cpuTemp = sensor.Value.GetValueOrDefault();
                            // print to console and append to data
                            float val = sensor.Value.GetValueOrDefault();
                            System.Diagnostics.Debug.WriteLine("cpuTemp: " + val);
                            data += " $" + int.Parse(val.ToString().Split('.')[0]);
                        }
                        else if (sensor.SensorType == SensorType.Load && sensor.Name.Contains("CPU Total") && CPUUsage)
                        {
                            // store
                            cpuUsage = sensor.Value.GetValueOrDefault();
                            // print to console and append to data
                            float val = sensor.Value.GetValueOrDefault();
                            System.Diagnostics.Debug.WriteLine("cpuUsage: " + val);
                            data += " $" + int.Parse(val.ToString().Split('.')[0]);
                        }
                        else if (sensor.SensorType == SensorType.Power && sensor.Name.Contains("CPU Package") && CPUPackage)
                        {
                            // store
                            cpuPowerDrawPackage = sensor.Value.GetValueOrDefault();
                            // print to console and append to data
                            float val = sensor.Value.GetValueOrDefault();
                            System.Diagnostics.Debug.WriteLine("CPU Power Draw - Package: " + val);
                            data += " $" + int.Parse(val.ToString().Split('.')[0]);

                        }
                        else if (sensor.SensorType == SensorType.Clock && sensor.Name.Contains("CPU Core #1") && CPUFrequency)
                        {
                            // store
                            cpuFrequency = sensor.Value.GetValueOrDefault();
                            // print to console and append to data
                            float val = sensor.Value.GetValueOrDefault();
                            System.Diagnostics.Debug.WriteLine("cpuFrequency: " + val);
                            data += " $" + int.Parse(val.ToString().Split('.')[0]);
                        }
                }


                // Targets AMD & Nvidia GPUS
                if (hardware.HardwareType == HardwareType.GpuAti || hardware.HardwareType == HardwareType.GpuNvidia)
                {
                    // only fire the update when found
                    hardware.Update();

                    // loop through the data
                    foreach (var sensor in hardware.Sensors)
                        if (sensor.SensorType == SensorType.Temperature && sensor.Name.Contains("GPU Core") && GPUTemp)
                        {
                            // store
                            gpuTemp = sensor.Value.GetValueOrDefault();
                            // print to console and append to data
                            float val = sensor.Value.GetValueOrDefault();
                            System.Diagnostics.Debug.WriteLine("gpuTemp: " + val);
                            data += "#GPUTemp: $" + int.Parse(val.ToString().Split('.')[0]);
                        }
                        else if (sensor.SensorType == SensorType.Load && sensor.Name.Contains("GPU Core")&&GPUUsage)
                        {
                            // store
                            gpuUsage = sensor.Value.GetValueOrDefault();
                            // print to console and append to data
                            float val = sensor.Value.GetValueOrDefault();
                            System.Diagnostics.Debug.WriteLine("gpuUsage: " + val);
                            data += "#GPUUsage: $" + int.Parse(val.ToString().Split('.')[0]);
                        }
                        else if (sensor.SensorType == SensorType.Clock && sensor.Name.Contains("GPU Core")&&GPUFrequency)
                        {
                            // store
                            gpuCoreFrequency = sensor.Value.GetValueOrDefault();
                            // print to console and append to data
                            float val = sensor.Value.GetValueOrDefault();
                            System.Diagnostics.Debug.WriteLine("gpuCoreFrequency: " + val);
                            data += "#GPUCoreFreq: $" + int.Parse(val.ToString().Split('.')[0]);
                        }
                        else if (sensor.SensorType == SensorType.Clock && sensor.Name.Contains("GPU Memory") && GPUMemory)
                        {
                            // store
                            gpuMemoryFrequency = sensor.Value.GetValueOrDefault();
                            // print to console and append to data
                            float val = sensor.Value.GetValueOrDefault();
                            System.Diagnostics.Debug.WriteLine("gpuMemoryFrequency: " + val);
                            data += "#GPUMemFreq: $" + int.Parse(val.ToString().Split('.')[0]);
                        }

                }

                // Can also gather fan data, motherboard, ram, and HDD data

            }
            return data;
        }

        static void Main(string[] args)
        {
            //Mutex mutex = new Mutex(false, "MySerialPortLock");
            var connection = new SharerConnection("COM5", 9600);
            connection.Connect();
            

            // loop
            while (true)
            {

                
                c.Open();
                // send a string to the serial port
                string message = ReportSystemInfo();
                //SerialPort serialPort = new SerialPort("COM5", 9600);
                //serialPort.DtrEnable = true;
                //serialPort.RtsEnable = true;
                //System.Diagnostics.Debug.WriteLine(message);

                // open the serial port
                //serialPort.Open();

                //serialPort.Write('1');
                connection.RefreshFunctions();
                connection.WriteVariable("MyVar", message); //send data containing parseable CPU/GPU stats
                // close the serial port
                //serialPort.Close();
                //return;
                //Thread.Sleep(2000);
                //mutex.ReleaseMutex();
            }
        }
    }
}