"""
* This Python Code is a Test Environment for I2C (Inter-Integrated Circuit) communication between a Raspberry Pi and an Arduino. This is just to simulate 
* how an Arduino would function when connected to the Jetson Nano.
* 
* This code will be modified such that it will use the concept of API Status calls, e.g. that error 404 and etc. you usually see online when you fail to load
* a webpage, to be used here by receiving those codes from the Arduino and responding to it.
* 
* Although the code could be utilised to send characters instead of numbers, it is overall better to use numbers so as to reduce bandwidth and keep
* communication speed high.
*
*     Version 2.2.2
*     Changes made:
* Implementation of f strings.
* Bug Fixes made to logging code which for some reason did not want to register other types of logging levels such as DEBUG or WARNING.
"""
import time
import logging
import os
from smbus2 import SMBus
logging.basicConfig(filename='I2CHealth.log', level=logging.DEBUG, format='%(asctime)s:%(levelname)s:%(message)s')
x = 0
y = 0


clientAddr = 0x08
bus = SMBus(1)

def i2cRead(): 
    for i in str(bus.read_byte(clientAddr)):
        data = bus.read_byte(clientAddr)
        time.sleep(0.905)
    logging.info("Message Read from Arduino")
    return int(data)

def i2cWrite(msg):
    for c in msg:
        bus.write_byte(clientAddr, ord(c))
    logging.info("Message Written to Arduino")
    return -1
        
def statusCodes():
    n = i2cRead()
    if n == 1:
        print("Arduino: Connection Established")
        logging.info("Arduino: Connection Established. Device Healthy")
    elif n == 2:
        print("Arduino: Data Received!")
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
        count +=1
        if count == 100:
            os.system('clear')
            count = 0
        statusCodes()
        print(i2cRead())



if __name__ == "__main__":
    try:
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
