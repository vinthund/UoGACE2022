/*
    Created by: Andrew Dean
    Description:
    This code contains the functions for homing the pan/tilt mechanism 
    and then driving it to target coordinates once it has homed.
*/

#include <AccelStepper.h>/* ~~ This code requires the AccelStepper library: https://github.com/waspinator/AccelStepper
                            The library's class references can found here: https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#details ~~ */
#define motorInterfaceType 1

/* ~~ Stepper pins ~~ */
const int ci_panStepPin = 25;
const int ci_panDirPin = 24;

const int ci_tiltStepPin = 13;
const int ci_tiltDirPin = 12;

/* ~~ Hall effect sensor pins ~~ */
const int ci_panHomeHallPin = 27;
const int ci_tiltHomeHallPin = 30;
const int ci_tiltDownHallPin = 32;
const int ci_tiltUpHallPin = 33;

/* ~~ Declare objects of AccelStepper to handle pan and tilt axes ~~ */
AccelStepper panStepper (motorInterfaceType, ci_panStepPin, ci_panDirPin);
AccelStepper tiltStepper (motorInterfaceType, ci_tiltStepPin, ci_tiltDirPin);

/* ~~ Variables ~~ */
bool b_serialDebugPrint = false; //enables serial debug text
bool b_panAxisHomed = false;
bool b_tiltAxisHomed = false;
int i_stateMachine = 0; //Store variable for state machine switch case
int i_tiltHomingDirection; //0 = was previously pointing up, 1 = was previously pointing down

int i_panTargetCoord = 0; //Store the target coordinate for the pan axis
int i_tiltTargetCoord = 0; //Store the target coordinate for the tilt axis

int i_panHomeHallDetect;
int i_tiltHomeHallDetect;
int i_tiltDownHallDetect;
int i_tiltUpHallDetect;

void setup()
{
    Serial.begin(9600);
    //Output pins
    pinMode(ci_panStepPin, OUTPUT);
    pinMode(ci_panDirPin, OUTPUT);

    pinMode(ci_tiltStepPin, OUTPUT);
    pinMode(ci_tiltDirPin, OUTPUT);

    //Input pins
    pinMode(ci_panHomeHallPin, INPUT);

    pinMode(ci_tiltHomeHallPin, INPUT);
    pinMode(ci_tiltDownHallPin, INPUT);
    pinMode(ci_tiltUpHallPin, INPUT);
}

void loop()
{
    switch(i_stateMachine)
    {
        case 0:
            homing_func();
            break;
        case 1:
            scanning_func();
            break;
        case 2:
            tracking_func();
            break;
        default:
            break;
    } //End of 'runState'
} //End of 'loop'

void homing_func() //Contains the function for homing the pan/tilt head
{
    if(b_tiltAxisHomed == false) // This section will home the tilt axis, it will run first.
    {
        i_tiltUpHallDetect = digitalRead(ci_tiltUpHallPin);
        i_tiltDownHallDetect = digitalRead(ci_tiltDownHallPin);
        i_tiltHomeHallDetect = digitalRead(ci_tiltHomeHallPin);
        i_panHomeHallDetect = digitalRead(ci_panHomeHallPin);

        tiltStepper.setSpeed(100);
        tiltStepper.setAcceleration(50);
        if(i_tiltUpHallDetect == 0)
        {
            i_tiltHomingDirection = 1;
            i_tiltTargetCoord = 133; /* If a tilt up/down hall effect is detected, axis is 30˚ from home
                                        Tilt stepper  = 1.8˚/step, 4:1 gearing ratio + half stepping = 133 steps to get back home */
            tiltStepper.stop(); //Stops the mech in the 
            tiltStepper.move(i_tiltTargetCoord); /* Drive CW back to approx tilt home hall effect,
                                                    The 'move' class variable will move the mech relative to the current position. */
            tiltStepper.run();
            if(b_serialDebugPrint = true)
            {
                Serial.println("Tilt axis pointing up, seeking tilt axis home hall effect sensor.");  
            }
        }

        if(i_tiltDownHallDetect == 0)
        {
            i_tiltHomingDirection = 2;
            i_tiltTargetCoord = 133;
            tiltStepper.stop(); 
            tiltStepper.move(-i_tiltTargetCoord); // Drive CCW back to approx tilt home hall effect
            tiltStepper.run();
            if(b_serialDebugPrint = true)
            {
                Serial.println("Tilt axis pointing down, seeking tilt axis home hall effect sensor.");  
            }
        }

        if(tiltStepper.distanceToGo() == 0 && i_tiltHomeHallDetect = 1) // if it has walked past the tilt home hall
        {
            if(i_tiltHomingDirection == 0)
            {
                i_tiltTargetCoord = 1;
                tiltStepper.move(i_tiltTargetCoord);
                i_tiltTargetCoord++;
                tiltStepper.run();
                if(b_serialDebugPrint == true)
                {
                    Serial.println("Tilt axis not at a limiting hall effect sensor, homing slowly...")
                }
            }
            
            if(i_tiltHomingDirection == 1)
            {
                i_tiltTargetCoord = 1;
                tiltStepper.move(i_tiltTargetCoord);
                i_tiltTargetCoord++;
                tiltStepper.run();
                if(b_serialDebugPrint == true)
                {
                    Serial.println("Tilt axis overshot tilt home hall effect, seeking tilt hall effect sensor.")
                }
            }
            
            if(i_tiltHomingDirection == 2)
            {
                i_tiltTargetCoord = 1;
                tiltStepper.move(-i_tiltTargetCoord);
                i_tiltTargetCoord++;
                tiltStepper.run();
                if(b_serialDebugPrint == true)
                {
                    Serial.println("Tilt axis overshot tilt home hall effect, seeking tilt hall effect sensor.")
                }
            }
        }

        if(i_tiltHomeHallDetect == 0) // If home hall effect is detected
        {
            tiltStepper.stop();
            tiltStepper.setCurrentPosition(0);
            if(b_serialDebugPrint == true)
            {
                Serial.println("Tilt axis homed!")
            }
            b_tiltAxisHomed = true;
        }
    }// end of tilt homing

    if(b_tiltAxisHomed == true) // Only enter this function once tilt axis has been homed
    {
        panStepper.setSpeed(100);
        panStepper.setAcceleration(50);
        
        if(i_panHomeHallDetect == 1) // If the pan hall effect is not detected
        {
            i_panTargetCoord = 1;
            panStepper.moveTo(i_panTargetCoord);
            i_panTargetCoord++;
            panStepper.run();
            if(b_serialDebugPrint == true)
            {
                Serial.println("Seeking pan axis home hall effect...");
            }
        }

        if(i_panHomeHallDetect == 0)
        { 
            panStepper.stop();
            panStepper.setCurrentPosition(0);
            if(b_serialDebugPrint == true)
            {
                Serial.println("Pan axis homed!")
            }
            b_panAxisHomed = true;
            i_stateMachine = 2; // Enter 'tracking_func' state
        }
    }
}// end of homing_func

void scanning_func() //Contains the function for 'scanning' for potential tartets
{

}

void tracking_func() //Contains the function for 'tracking' a target once it has been identified
{
    if(Serial.available() > 0)
    {
        /* 
            Add code here that accepts users coordinates
            Checks to see if inputted coordinates are within bounds of acceptable movement
            And drives mechanism to user specified coordinates
        */
       if(i_tiltTargetCoord > -133 || i_tiltTargetCoord > 133)
       {
           Serial.println("Please enter a value between -133 (Look up) and 133 (Look down).");
       }
    }
} //End of tracking_func