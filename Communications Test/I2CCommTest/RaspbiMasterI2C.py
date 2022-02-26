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
* Version 1.1.1
"""

import time
from smbus import SMBus

clientAddr = 0x08
bus = SMBus(1)
encoding = 'utf-8'

def i2cWrite(msg):
  for c in msg:
    bus.write_byte(clientAddr, ord(c))
  return -1

def i2cRead():
    for i in range(0, 20):
        data = bus.read_byte(clientAddr)
    print("From Arduino: %02d" % data)

def main():
  print("Send message to Arduino")
  while True:
    msg = input("> ")
    print("...")
    i2cWrite(msg)
    i2cRead()
    

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Program Terminated Manually")
