#!/usr/bin/env python

# Serial port testing
import serial
import time

test_string = b'12342134123412341234'
port_list = ["/dev/ttyS0", "/dev/ttyS0",] # add other connectors if you want

for port in port_list:
#115200
    try:
        serialPort = serial.Serial(port, 115200, timeout = 10)
        print(f"Opened port {port} for testing:")
        serialPort.flush()
        bytes_sent = serialPort.write(test_string)
        print(f"Sent {bytes_sent} bytes")
        loopback = serialPort.readline(bytes_sent)
        if loopback == test_string:
            print(f"Received {len(loopback)} valid bytes, Serial port {port} working \n")
        else:
            print(f"Received incorrect data {loopback} over Serial port {port} loopback\n")
        serialPort.close()
    except IOError:
        print(f"Failed at {port} \n")



