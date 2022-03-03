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
* Version 1.5.1
* Recent Amendments: Removal of encoding variable, amendment made to statusCode function, statusCodes does not work as of yet.
* Status Codes function works similar to API Status calls, so instead of sending words over I2C, numbers are instead sent and these numbers are given conditions. 
"""

import time
from smbus import SMBus


clientAddr = 0x08
bus = SMBus(1)

def i2cWrite(msg):
  for c in msg:
    bus.write_byte(clientAddr, ord(c))
  return -1

def i2cRead():
    while True:
        data = bus.read_byte(clientAddr)
        #print("From Arduino: %02d" % data)
        time.sleep(1)
        
def statusCodes():
    n = i2cRead()
    if n == 1:
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
    

def main():
  print("Send message to Arduino")
  while True:
    #print(data)
    msg = input("> ")
    print("...")
    i2cWrite(msg)
    #i2cRead()
    #statusCodes()
    

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Program Terminated Manually")
 