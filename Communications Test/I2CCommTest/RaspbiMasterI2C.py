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
*    Version 2.0.4
* Changes made:
* Error Code adjustment; implementation of Error 4: The reason this came to be was due to 4 always being printed to the
(continuation) terminal whenever there was some sort of communication error. Adding this is good as it will point out
(continuation) any communication error on I2C.
*
* Implementation of Heartbeat Signal: The i2cWrite function is now sending a value over I2C to the Arduino,
(continuation) so as to act as a heartbeat signal. It is sending the value "100", which means "Status OK"
*
* Bug Fixes: The statusCodes function had 100 accidentally set as a string. The program never picked up on this and
(continuation) would often ignore a "100" from the Arduino.
"""

import time
import os
from smbus2 import SMBus
count = 0


clientAddr = 0x08
bus = SMBus(1)

def i2cRead(): 
    for i in str(bus.read_byte(clientAddr)):
        data = bus.read_byte(clientAddr)
        time.sleep(0.905)
    return int(data)

def i2cWrite(msg):
    for c in msg:
        bus.write_byte(clientAddr, ord(c))
    return -1
        
def statusCodes():
    n = i2cRead()
    if n == 1:
        print("Arduino: Connection Established") 
    elif n == 2:
        print("Arduino: Data Received!")
    elif n == 4:
        print("Bi-Directional Communication Error")
    elif n == 21:
        print("Arduino: Out of Ammo!")
    elif n == 22:
        print("Shooting Error! Fix Immediately!")
    elif n == 100:
        print("Arduino: Currently Alive!")
    elif n == 110:
        print("Arduino: Program Error!")
    elif n == 115:
        print("Arduino: Mechanical Error!")
    else:
        print("Arduino Status Unknown")

    

def main():
    count = 0
    while True:
        i2cWrite("100")
        count +=1
        if count == 10:
            os.system('clear')
            i2cWrite("5")
            count = 0
        #msg = input("Send message to Arduino \n" + "> ")
        #print("...")
        statusCodes()
        print(i2cRead())
        #i2cWrite(msg)

    

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Program Terminated Manually")
 
