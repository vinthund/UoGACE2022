#machine vision implementation
#see readme for greater explanation
#sian pugh sp3048k vinthund@github
#version 1.0.0

import numpy
import cv2
import smbus
import flask

video=cv2.VideoCapture(0)
comms=smbus.SMBus(1)
address=0x8
regmode = 0x00
ledout = 0x1d

background=None
#background is background of vision, used to subtract foreground for motion detection

while True:
    frame=video.read()
    motion=0
    #when motion = 0, no motion detected
    grey = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #convert first captured frame to greyscale for easier processing
    if background is None:
        background = grey
        #sets first frame as the background still
        continue
    nextframe=cv2.absdiff(still, grey)
    #contains the difference between the current frame and the background still
    framecutout = cv2.threshold(nextframe, 20, 255, cv2.THRESH_BINARY)[1]
    #if change detected, then it is coloured white (255)

    movecontour = cv2.findContours(framecutout.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    for contour in movecontour:
        if cv2.contourArea(contour) < 100000:
            continue
        motion = 1
        #sets motion to 1, meaning motion is detected

        (x, y, width, height) = cv.boundingRect(contour)
        #draw rectangle around movement
        cv2.rectangle(frame, (x, y), (x + width, y + height), (0, 255, 0), 3)

        centerheight=height/2
        centerwidth=width/2
        #used to find center of rectangle
        frameheight, framewidth = frame.shape[:2]
        #find height and width of full image; first two values in frame array
        centerframeheight=frameheight/2
        centerframewidth=framewidth/2
        #finds coordinates of middle of image
        centerrectanglex=centerheight+x
        centerrectangley=centerwidth+y
        #finds coordinates of middle of rectange
        xmove=centerrectanglex-frameheight
        ymove=centerrectangley-frameheight
        #difference between the two to send to arduino

        values=[xmove, ymove]
        bus.write_i2c_block_data(address, ledout, values)
        #send via i2c to arduino
    
        
