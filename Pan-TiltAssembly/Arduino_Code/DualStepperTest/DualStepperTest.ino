/*
    Created by: Andrew Dean~~~~~~~~~~~~~~~~~~~~~~~~~
    This code is a follow on from SingleStepperTest.
    This code controls both the pan and tilt axes.~~
*/

#include <AccelStepper.h> ////This code requires the AccelStepper library: https://github.com/waspinator/AccelStepper
                          //The library's class references can found here: https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#details

#define motorInterfaceType 1
 
const int ci_panDirectionPin = 2;
const int ci_panSteppingPin = 3;

const int ci_tiltDirectionPin = 4;
const int ci_tiltSteppingPin = 5;

AccelStepper panStepper (motorInterfaceType, ci_panSteppingPin, ci_panDirectionPin);
AccelStepper tiltStepper (motorInterfaceType, ci_tiltSteppingPin, ci_tiltDirectionPin);

int i_panTargetPosition = 200; //Distance to target angle in steps
int i_tiltTargetPosition = 54; 
/*
--| The NEMA17 steppers have a step angle of 1.8˚, meaning 200 steps are required to rotate through 360˚.
--| The gearing of the pan and tilt axes is 4:1, meaning the step angle on the output gear is reduced to 0.45˚/step.
--| Therefore, desired target angle / 0.45 = Number of steps to reach desired target angle.
*/

void setup()
{
    //Pan Stepper Parameters
    panStepper.setMaxSpeed(1000.0);
    panStepper.setAcceleration(25.0); //Acceleration rate in steps/s/s
    panStepper.setSpeed(50.0); //Target speed in steps/s

    //Tilt Stepper Parameters
    tiltStepper.setMaxSpeed(1000.0);
    tiltStepper.setAcceleration(50.0); //Acceleration rate in steps/s/s
    tiltStepper.setSpeed(100.0); //Target speed in steps/s
}

void loop()
{
    //Pan Stepper Control
    if(panStepper.distanceToGo() == 0 )
    {
        panStepper.moveTo(i_panTargetPosition);
    }

    if(panStepper.distanceToGo() == i_panTargetPosition)
    {
        panStepper.moveTo(-i_panTargetPosition);
    }
    
    //Tilt Stepper Control
    if(tiltStepper.distanceToGo() == 0 )
    {
        tiltStepper.moveTo(i_tiltTargetPosition);
    }

    if(panStepper.distanceToGo() == i_tiltTargetPosition)
    {
        panStepper.moveTo(-i_tiltTargetPosition);
    }

    panStepper.run();
    tiltStepper.run();
    
} //end of 'loop'