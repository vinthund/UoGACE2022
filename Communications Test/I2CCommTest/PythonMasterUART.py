"""
* UART Version of I2C Code due to I2C already being used for a sensor device on the Arduino.
*
*     Version 0.4.0
*     Implementations made:
* Revamping old system of I2C into UART due to inability to use I2C bus on Arduino, as it is being used by another device.
* Revamps include implementing a decode function to translate the data being transmitted over UART.
* All messages get encoded before getting sent to Arduino in i2cWrite.
* Utilisation of new Log file named UARTHealthLog
* Clear range increased to 300.
"""
import time
import logging
import os
import serial
logging.basicConfig(filename='UARTHealth.log', level=logging.DEBUG, format='%(asctime)s:%(levelname)s:%(message)s')
x = 0
y = 0


def i2cRead():
    #if ser.in_waiting > 0:
            #data = ser.readline().decode('utf-8').rstrip()
    data = ser.readline().decode('utf-8').rstrip()
    logging.info("Message Read from Arduino")
    return int(float(data))

def i2cWrite(msg):
    #for c in msg:
        #ser.write(ord(c))
    msg = str.encode(msg)
    ser.write(msg)
    logging.info("Message Written to Arduino")
    return -1
        
def statusCodes():
    n = i2cRead()
    if n == 1:
        print("Arduino: Connection Established")
        logging.info("Arduino: Connection Established. Device Healthy")
    elif n == 2:
        print("Arduino: Data Received!")
        logging.info("Arduino: Connection Established. Device Healthy")
    elif n == 21:
        print("Arduino: Out of Ammo!")
        logging.warning("Arduino: Out of Ammo!")
    elif n == 22:
        print("Shooting Error! Fix Immediately!")
        logging.warning("Arduino: Shooting Error!")
    elif n == 100:
        print("Arduino: Currently Alive!")
        logging.info("Arduino: Device Healthy")
    elif n == 110:
        print("Arduino: Program Error!")
        logging.warning("Arduino: Program Error")
    elif n == 115:
        print("Arduino: Mechanical Error!")
        logging.warning("Arduino: Mechanical Error")
    else:
        print("Arduino Status Unknown")
        logging.info("Arduino Device Status Unknown.")
        
def setCoordinate(x_coord, y_coord):
    coord = (f'({x_coord},{y_coord})')
    i2cWrite(coord)
    logging.info(f'Coordinate: {coord}')

def main():
    count = 0 
    while True:
        setCoordinate(450, 0)
        #i2cWrite("100")
        count +=1
        if count == 300:
            os.system('clear')
            count = 0
        statusCodes()
        print(i2cRead())



if __name__ == "__main__":
    try:
        ser = serial.Serial('/dev/ttyS0', 9600, timeout = 100)
        ser.flush
        main()
    except KeyboardInterrupt:
        print("Program Terminated Manually")
        logging.info("Program Terminated Manually")
    except IOError:
        while True:
            time.sleep(0.5)
            try:
                print("Error 4 - Arduino Disconnected")
                logging.warning("Arduino Device Disconnected.")
                main()
            except:
                pass
