Motion Detection in Python
Version 0.0.1
Written By Sian Pugh, sp3045k@gre.ac.uk, fleetfootgreyhound@gmail.com, vinthund @ github

Simple backend motion detector written in Python. Intended for use on a Jetson Nano 2gb to communicate with an Arduino system. 

Part of a larger project used to control an autonomous nerf sentry - https://github.com/vinthund/UoGACE2022

Currently uses a subtractive method; it will take the first frame of input and store it as the "background", then compare all following frames to this. It has a threshold mechanism in place to ensure that small differences such as shadows are not seen as motion.
When motion is detected using this subtractive method, a countour of the motion is drawn, as well as a retangle constructed of the area around it. This rectangle is used for specific targetting, as the center of it is found and then communicated via i2c to the arduino unit.
Updates needed:
-Mechanism of updating the background when the camera is moved (possibly a database of images of the surrounding area according to pan/tilt?)
-RTSP stream to website/app

Dependencies:
This program requires the numpy and opencv packages to function. 
https://numpy.org/
https://opencv.org/

