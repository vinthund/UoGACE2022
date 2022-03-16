#include <Servo.h>
#include <Ramp.h>
#include <Wire.h>
#include <AS5600.h>
#include <AccelStepper.h>

//Definitions
#define motorInterfaceType 1
#define vmaxPWM 30

//Stepper pins
const int ci_magStepPin = 2;
const int ci_magDirPin = A2;
const int ci_panStepPin = 4;
const int ci_panDirPin = 3;
const int ci_tiltStepPin = A0;
const int ci_tiltDirPin = A1;

//Servo Pins
const int ci_servoPin = 10;

//Motor Pins
const int ci_motorPin = 5;

//Hall effect pins
const int ci_panHomeHallPin = 6;
//const int ci_panLeftHallPin = 7; // Not used
//const int ci_panRghtHallPin = 8; // Not used
const int ci_tiltHomeHallPin = 11; 
const int ci_tiltDownHallPin = 9;
const int ci_tiltUpHallPin = 8;

//Declarations
Servo magServo;
ramp motorRamp;
AMS_5600 magSensor;
AccelStepper panStepper (motorInterfaceType, ci_panStepPin, ci_panDirPin);
AccelStepper tiltStepper (motorInterfaceType, ci_tiltStepPin, ci_tiltDirPin);
AccelStepper magStepper (motorInterfaceType, ci_magStepPin, ci_magDirPin);

//Constants
const int ci_dartPos[12] = { 250 , 568, 938, 1275, 1609, 1973, 2293, 2616, 2949, 3332, 3668, 3991 };
const byte cb_servoIn = 130;
const byte cb_servoOut = 10;
const byte cb_vMaxPWM = 30;
const int ci_motorSpinUpPeriod = 5000;
const int ci_maxMagSpeed = 100;

//Variables
bool b_serialDebugPrint = false; //enables serial debug text
bool b_panAxisHomed = false;
bool b_tiltAxisHomed = false;
int i_stateMachine = 0; //Store variable for state machine switch case
int i_tiltHomingDirection; //0 = was previously pointing up, 1 = was previously pointing down
int i_panTargetCoord = 0; //Store the target coordinate for the pan axis
int i_tiltTargetCoord = 0; //Store the target coordinate for the tilt axis
int i_panHomeHallDetect;
//int i_panLeftHallDetect; // Not used
//int i_panRghtHallDetect; // Not used
int i_tiltHomeHallDetect;
int i_tiltDownHallDetect;
int i_tiltUpHallDetect;



//dev
int targetPos = 54; //Target position in steps


void setup() {
  //Comms Setup
  Serial.begin(9600);
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  Serial.println("Machine Vision Foam Dart Sentry");
  Serial.println("-------------------------------");
  Wire.begin();

  //Firing Motor Setup
  digitalWrite(ci_motorPin, 0);

  //Servo Pin Setup
  magServo.attach(ci_servoPin);
  magServo.write(cb_servoOut);

  //Stepper Setups
  magStepper.setMaxSpeed(ci_maxMagSpeed);

  //Output pins
  pinMode(ci_panStepPin, OUTPUT);
  pinMode(ci_panDirPin, OUTPUT);
  pinMode(ci_tiltStepPin, OUTPUT);
  pinMode(ci_tiltDirPin, OUTPUT);
  pinMode(ci_magStepPin, OUTPUT);
  pinMode(ci_magDirPin, OUTPUT);
  pinMode(ci_motorPin, OUTPUT);

  //Input pins
  pinMode(ci_panHomeHallPin, INPUT);
  //pinMode(ci_panLeftHallPin, INPUT); // Not used
  //pinMode(ci_panRghtHallPin, INPUT); // Not used
  pinMode(ci_tiltHomeHallPin, INPUT);
  pinMode(ci_tiltDownHallPin, INPUT);
  pinMode(ci_tiltUpHallPin, INPUT);



  //Dev
  panStepper.setMaxSpeed(1000.0); //This sets the maximum speed, datatype is float.
    panStepper.setAcceleration(25.0); //This sets the acceleration and deceleration rates in steps/s, datatype is float.
    panStepper.setSpeed(50.0);
    tiltStepper.setMaxSpeed(1000.0); //This sets the maximum speed, datatype is float.
    tiltStepper.setAcceleration(25.0); //This sets the acceleration and deceleration rates in steps/s, datatype is float.
    tiltStepper.setSpeed(50.0);

    test_all();
  
}

//Loop Function
void loop() {

}


//---------------- Custom Functions ----------------\\


void test_all(){
  int i = 0;
  while(i < 2000){
    if(panStepper.distanceToGo() == 0) //distanceToGo returns the distance in steps from the current position in steps to that set by moveTo.
    {
        panStepper.moveTo(targetPos); 
    }
    
    if(panStepper.currentPosition() == targetPos) //currentPosition calls the stepper motors current position.
    {
        panStepper.moveTo(-targetPos); //Setting a negative moveTo distance will drive it past the 0 (home) point, wherever that may be.
    }
    panStepper.run();
    delay(1);
    i++;
  }

  i = 0;
  while(i < 2000){
    if(tiltStepper.distanceToGo() == 0) //distanceToGo returns the distance in steps from the current position in steps to that set by moveTo.
    {
        tiltStepper.moveTo(targetPos); 
    }
    
    if(tiltStepper.currentPosition() == targetPos) //currentPosition calls the stepper motors current position.
    {
        tiltStepper.moveTo(-targetPos); //Setting a negative moveTo distance will drive it past the 0 (home) point, wherever that may be.
    }
    tiltStepper.run();
    delay(1);
    i++;
  }
  
  home_mag();
  spinUp();
  fire();
  spinDown();
}

void home_mag() //Contains the function for homing the magazine to dart zero
{
   goToDart(0);
}

void goToDart(byte dart) //Contains the function for selecting a dart
{
  while(1)
  {
    delay(30);
    int i_tmp = magSensor.getRawAngle();
    int i_d = ci_dartPos[dart] - i_tmp;
    if ( i_tmp > (ci_dartPos[dart] - 3) && i_tmp < (ci_dartPos[dart] + 3)) {
      return 0;
    }
    else if ( i_tmp > ci_dartPos[dart]) {
      magStepper.setSpeed(50); 
      magStepper.runSpeed();
    }
    else {
      magStepper.setSpeed(-50); 
      magStepper.runSpeed();
    }
  }
}

void spinUp() //Contains the function for spinning up the firing motors
{
  motorRamp.go(0);
  motorRamp.go(cb_vMaxPWM, ci_motorSpinUpPeriod);
  while(motorRamp.update() != cb_vMaxPWM)
  {
    analogWrite(ci_motorPin, motorRamp.update());
  }
}

void spinDown() //Contains the function for spinning down the firing motors
{
  analogWrite(ci_motorPin, 0);
}

void fire() //Contains the function to fire the selected dart
{
  magServo.write(cb_servoIn);
  delay(400);
  magServo.write(cb_servoOut);
  delay(400);
}

void homing_func() //Contains the function for homing the pan/tilt head
{
    if(b_tiltAxisHomed == false) // This section will home the tilt axis, it will run first.
    {
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

        if(tiltStepper.distanceToGo() == 0 && i_tiltHomeHallDetect == 1) // if it has walked past the tilt home hall
        {
            if(i_tiltHomingDirection == 0)
            {
                i_tiltTargetCoord = 1;
                tiltStepper.move(i_tiltTargetCoord);
                i_tiltTargetCoord++;
                tiltStepper.run();
                if(b_serialDebugPrint == true)
                {
                    Serial.println("Tilt axis not at a limiting hall effect sensor, homing slowly...");
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
                    Serial.println("Tilt axis overshot tilt home hall effect, seeking tilt hall effect sensor.");
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
                    Serial.println("Tilt axis overshot tilt home hall effect, seeking tilt hall effect sensor.");
                }
            }
        }

        if(i_tiltHomeHallDetect == 0) // If home hall effect is detected
        {
            tiltStepper.stop();
            tiltStepper.setCurrentPosition(0);
            if(b_serialDebugPrint == true)
            {
                Serial.println("Tilt axis homed!");
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
                Serial.println("Pan axis homed!");
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
