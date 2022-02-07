#Note: This Code is only a Test Environment for Serial Communication to the Arduino. Commands are sent from here through Serial in order to control the Arduino
#Currently, this code sends commands to control the spin direction of a fan conncted to an H bridge, when the command is "positive" it spins clockwise, and "negative" spins
#counter clockwise. Of course, these commands are only sent to the Arduino when the conditions are achieved.
#Version 1.1.3

import serial
import time
from time import sleep

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout = 1)
    ser.flush
    
    while True:
        #time.sleep(0.1)
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line, flush = True)
            if line.isdigit():
                if float(line) > 600:
                    ser.write(b"positive\n")
                elif float(line) < 400:
                    ser.write(b"negative\n")
                elif float(line) < 600 or float(line) > 400:
                    ser.write(b"off\n")
            else:
                if str(line) == "Requesting Connection":
                    print("Device Has Been Acknowledged\n")
                    ser.write(b"Connection Established\n")
