#include <AccelStepper.h>
#define motorInterfaceType 1

//Stepper pins
const int ci_panStepPin = 25;
const int ci_panDirPin = 24;

const int ci_tiltStepPin = 13;
const int ci_tiltDirPin = 12;

//Hall effect pins
const int ci_panHomeHallPin = 27;
//const int ci_panLeftHallPin = 28; // Not used
//const int ci_panRghtHallPin = 29; // Not used

const int ci_tiltHomeHallPin = 30;
const int ci_tiltDownHallPin = 32;
const int ci_tiltUpHallPin = 33;

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
    //pinMode(ci_panLeftHallPin, INPUT); // Not used
    //pinMode(ci_panRghtHallPin, INPUT); // Not used
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