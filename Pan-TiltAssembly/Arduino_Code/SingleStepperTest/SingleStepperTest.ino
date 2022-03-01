/*
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Created by: Andrew Dean ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    This code is for quickly testing the funcitonality of the stepper motors on the pan/tilt head.
    A A4988 stepper driver was used to drive the stepper motor for this test. ~~~~~~~~~~~~~~~~~~~~
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

#include <AccelStepper.h> //This code requires the AccelStepper library: https://github.com/waspinator/AccelStepper
                          //The library's class references can found here: https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#details

const int ci_directionPin = 2; //This pin is used to control which direction the stepper motor is rotating.
const int ci_steppingPin = 3;  //This pin is used to output the stepping pulses to the A4988.

#define motorInterfaceType 1 //As an A4988 is being used to drive the stepper motor, motorInterfaceType 1 is used to output stepping pulses to the steppingPin.

AccelStepper testStepper (motorInterfaceType, ci_steppingPin, ci_directionPin); //This creates an instance of the library called testStepper

int targetPos = 54; //Target position in steps

void setup()
{
    testStepper.setMaxSpeed(1000.0); //This sets the maximum speed, datatype is float.
    testStepper.setAcceleration(25.0); //This sets the acceleration and deceleration rates in steps/s, datatype is float.
    testStepper.setSpeed(50.0); //This sets the desired speed in steps per second, datatype is float.
}

void loop()
{
    if(testStepper.distanceToGo() == 0) //distanceToGo returns the distance in steps from the current position in steps to that set by moveTo.
    {
        testStepper.moveTo(targetPos); 
    }
    
    if(testStepper.currentPosition() == targetPos) //currentPosition calls the stepper motors current position.
    {
        testStepper.moveTo(-targetPos); //Setting a negative moveTo distance will drive it past the 0 (home) point, wherever that may be.
    }
    
    testStepper.run(); //This function must 
    
}
