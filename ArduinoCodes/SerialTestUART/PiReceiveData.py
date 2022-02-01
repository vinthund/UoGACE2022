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
            if float(line) > 600:
                ser.write(b"positive\n")
            elif float(line) < 400:
                ser.write(b"negative\n")
            elif float(line) < 600 or float(line) < 400:
                ser.write(b"off\n")
        