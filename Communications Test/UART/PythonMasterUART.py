"""
* UART Version of I2C Code due to I2C already being used for a sensor device on the Arduino.
*
*     Version 0.8.4
*     Implementations made:
* Added a conditional statement that checks if data is either string or integer through "'ammo' in data".
* Added new variable ammo to store ammo value being sent over Serial.
* Added new condition within StatusCodes to detect if it is first receiving a string through 'isinstance' then spliting and taking out the numbers within said string.
* Added a new log to log remaining ammo.
* Re-organised numbering structure of errors and status within statuscode function.
"""
#Importing time module for delays, as they are needed to stop flooding the terminal and Arduino terminal with information.
import time
#Importing logging to allow for data to be logged into a file. This is useful to check for data occuring when the program is running, and
#levels can even be set
import logging
#OS imported to allow for operating system commands to me used. This is just to be used to clear the terminal so that the device does not lag.
import os
#Serial Imported to allow for communication over Serial. In this case, UART is used(Rx Tx).
import serial

#Logging is set through basic config, which is saved in a file named UARTHealth, the level is set at debugging to allow
#for other seperate levels such as warning and info to be sent on other levels. Setting it as info does not allow for other levels to be used.
#This is formatted in a time, level and message manner, similar to a log on a windows machine.
logging.basicConfig(filename='UARTHealth.log', level=logging.DEBUG, format='%(asctime)s:%(levelname)s:%(message)s')

#X and Y variables, to be used to store Machine Vision's coordinates. 
x = 0
y = 0

#Deals with storing the ammo left within the turret, receives it from Arduino over Serial.
ammo = 0

#Below function deals with reading data from the Arduino, it first decodes the oncoming bytes into unicode to be make them understandable.
#This information is then logged into the Log file, along with the message contents.
#The data is then converted into float, then to int to increase its numerical range. Just using int will not work.
def UARTRead():
    data = ser.readline().decode('utf-8').rstrip()
    logging.info(f'Message from Arduino: {data}')
    #Below Condition checks if the string "Ammo" is within the data being read,
        #if it is, then it will return it as a string for the ammo reading function, else it will return it as an integer for normal statuses
    if "Ammo" in data:
        return str(data)
    else:
        return int(float(data))

#Below function deals with writing a message to the Arduino through string encoding. It accepts the parameter "msg" so that whenever it is called,
#it can take any message and encode it as a string. Using it without encoding raises a TypeError.
def UARTWrite(msg):
    logging.info(f'Message Written to Arduino: {msg}')
    msg = str.encode(msg)
    ser.write(msg)
    return -1

#Below function deals with receiving the numerical information sent from the Arduino. This is inspired by API Status Calls which often use
#numbers to define their errors, like Error 404 which is the most common one. This function defines a variable "n" and assigns whatever is
#returned into UARTRead into variable n. If this value is equal to any of the conditional statements, then it will print this to the terminal, and
#log said data. If this number being sent does not exist, or if it is a string, it will throw "Status Unknown".
def statusCodes(): 
    n = UARTRead()
    #Allowing ammo to be accessed everywhere
    global ammo
    
    #IsInstance is used to check if the data within "n" is a string, and if it is, go through a loop that takes only
        #the number being sent over Serial.
    if isinstance(n, str):
        lst = n.split()
        for i in lst:
            ammo = ''
            for j in i: # To extract only numbers from the word
                if j.isdigit():
                    ammo+=j
        #print(ammo)
        logging.info(f'Ammo: {ammo} Remaining')
    elif n == 99:
        print("Arduino: Connection Established!")
        logging.info("Arduino: Connection Established. Device Healthy")
    elif n == 100:
        print("Arduino: Currently Alive!")
        logging.info("Arduino: Device Healthy")
    elif n == 101:
        print("Arduino: Data Received!")
        logging.info("Arduino: Data Received!")
    elif n == 150:
        print("Arduino: Program Error!")
        logging.warning("Arduino: Program Error")
    elif n == 200:
        print("Arduino: Movement Error!")
        logging.warning("Arduino: Movement Error")
    elif n == 201:
        print("Arduino: Shooting Error!")
        logging.warning("Arduino: Shooting Error") 
    else:
        print("Arduino Status Unknown")
        logging.info("Arduino Device Status Unknown.")
        
  
        
#The below function setCoordinate deals with the parameters "x_coord" and "y_coord" with take in whatever coordinates are being received from
        #the machine vision. The values are then stored in a variable named "coord", where the string literal "f" is used to make it look
        #nice to then send the data to the Arduino using the UARTWrite function. The data is then logged of what was written.
def setCoordinate(x_coord, y_coord):
    coord = (f'({x_coord},{y_coord})')
    UARTWrite(coord)
    logging.info(f'Coordinate: {coord}')

#Below function is the main, which deals with running the main program. Ser is defined to be the Serial Port connected over UART S0, with
    #baud 9600 and timeout 100, so that it does not timeout quickly. Variable count is defined too to allow for counts to be made so that the
    #terminal can be kept clean. If the terminal is not maintained and cleaned after a certain time, it will start to lag the program.
    #Currently, coordinates are being set using the setCoordinate function, along with printing the data being sent from the Arduino into
    #the terminal.
def main():
    count = 0 
    while True:
        setCoordinate(450, 0)
        #UARTWrite("100")
        count +=1
        if count == 300:
            os.system('clear')
            count = 0
        statusCodes()
        #Printing ammo to test if its really global.
        print(ammo)
        #print(UARTRead())



#The below makes sure to run the module "main" and not the imported modules. It will "try" main. In case main suddenly fails, exceptions will
        #be made to catch whatever is causing errors to be raised, such as KeyboardInterrupt and IOError. Keyboard allows for the program to break
        #whenever Control+C is pressed.
        #IOError exception catches the exception of an IOError, thus it will not automatically break the program, rather it will keep trying main
        #until it the Arduino is re-connected.
if __name__ == "__main__":
    try:
        ser = serial.Serial('/dev/ttyS0', 9600, timeout = 10)
        ser.flush
        main()
    except KeyboardInterrupt:
        print("Program Terminated Manually")
        logging.info("Program Terminated Manually")
    #If there is a ValueError or IOError, it will keep trying main until it becomes active again.
    except ValueError or IOError:
        while True:
            time.sleep(0.1)
            try:
                print("Error 4 - Arduino Disconnected")
                logging.warning("Arduino Device Disconnected.")
                main()
            except:
                pass
