#include <AccelStepper.h>
#define motorInterfaceType 1

//Stepper pins
const int ci_panStepPin = 2;
const int ci_panDirPin = 3;

const int ci_tiltStepPin = 4;
const int ci_tiltDirPin = 5;

//Hall effect pins
const int ci_panHomeHallPin = 6;
const int ci_panLeftHallPin = 7;
const int ci_panRghtHallPin = 8;

const int ci_tiltHomeHallPin = 9;
const int ci_tiltDownHallPin = 10;
const int ci_tiltUpHallPin = 11;

//Declare objects of AccelStepper to handle pan and tilt axes
AccelStepper panStepper (motorInterfaceType, ci_panStepPin, ci_panDirPin);
AccelStepper tiltStepper (motorInterfaceType, ci_tiltStepPin, ci_tiltDirPin);

//Variables
bool serialDebugPrint = false; //enables serial debug text
int stateMachine = 0; //Store variable for state machine switch case

int i_panHomeHallDetect;
int i_panLeftHallDetect;
int i_panRghtHallDetect;

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
    pinMode(ci_panLeftHallPin, INPUT);
    pinMode(ci_panRghtHallPin, INPUT);
    pinMode(ci_tiltHomeHallPin, INPUT);
    pinMode(ci_tiltDownHallPin, INPUT);
    pinMode(ci_tiltUpHallPin, INPUT);
}

void loop()
{
    switch(stateMachine)
    {
        case(0):
            homing_func();
            break;
        case(1):
            scanning_func();
            break;
        case(2):
            tracking_func();
            break;
        default:
            break;
    } //End of 'runState'
} //End of 'loop'

void homing_func() //Contains the function for homing the pan/tilt head
{

}

void scanning_func() //Contains the function for 'scanning' for potential tartets
{

}

void tracking_func() //Contains the function for 'tracking' a target once it has been identified
{

}