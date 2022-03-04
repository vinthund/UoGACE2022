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
* Version 1.9.2
* Changes made:
* Printing from Arduino, removal of redundancy by putting "send message to arduino" on input prompt.
* Replacement of while loop in i2cRead function to for loop, to increase program efficiency and lower risk of program crash.
* Additional changes made to i2cRead funtion being conversion of data variable into an integer. This is for the - 
(continuation) sole purpose of using the output numbers for the status codes function.
* Bug Fixes: Removal of "print" within i2cRead function due to it breaking code in earlier instances, as it could not convert
* data to integer when called elsewhere from code.
* Addition of OS module for communication between Python and Operating System, this is utilised in this instance-
(continuation) to clear the terminal to reduce system slow downs.
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
        time.sleep(0.5)
    return int(data)

def i2cWrite(msg):
    for c in msg:
        bus.write_byte(clientAddr, ord(c))
    return -1
        
def statusCodes():
    n = i2cRead()
    if n == 0:
        print("Arduino: Connection Established")
    elif n == 2:
        print("Arduino: Data Received!")
    elif n == 21:
        print("Arduino: Out of Ammo!")
    elif n == 22:
        print("Shooting Error! Fix Immediately!")
    elif n == "100":
        print("Arduino: Currently Alive!")
    elif n == 110:
        print("Arduino: Program Error!")
    elif n == 115:
        print("Arduino: Mechanical Error!")
    else:
        print("Arduino Status Unknown")
        #time.sleep(1)
    

def main():
    count = 0
    while True:
        count +=1
        if count == 10:
            #os.system('cls' if os.name == 'nt' else 'clear')
            os.system('clear')
            count = 0
        #i2cRead()
        #msg = input("Send message to Arduino \n" + "> ")
        #print("...")
        #statusCodes()
        print(i2cRead())
        #i2cWrite(msg)

    

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Program Terminated Manually")
 