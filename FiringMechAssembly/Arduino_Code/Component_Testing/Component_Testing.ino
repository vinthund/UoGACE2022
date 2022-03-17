#include <Servo.h>        // Standard Arduino library for controlling servos
#include <Ramp.h>         // Standard Arduino library used to interpolate between values
#include <Wire.h>         // Standard Arduino I2C library
#include <AS5600.h>       // AS5600 library can be found here: https://github.com/Seeed-Studio/Seeed_Arduino_AS5600 
#include <AccelStepper.h> // AccelStepper library can be found here: https://github.com/waspinator/AccelStepper
                          // AccelStepper's class references can be found herehttps://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#details 

//Definitions
#define motorInterfaceType 1 // As stepper drivers are being used, this will configure AccelStepper to just send step pulses on a single output pin
#define vmaxPWM 30

//Stepper pins
const int ci_magStepPin = 2; // Defines the step pulse output pin
const int ci_magDirPin = A2; // Defines the direction output pin, low = move CCW, high = move CW

const int ci_panStepPin = 4;
const int ci_panDirPin = 3;
const int ci_tiltStepPin = A0;
const int ci_tiltDirPin = A1;

//Servo Pins - For firing mech
const int ci_servoPin = 10;

//Motor Pins - For firing mech
const int ci_motorPin = 5;

//Hall effect pins
const int ci_panHomeHallPin = 6;
//const int ci_panLeftHallPin = 7; // Not used
//const int ci_panRghtHallPin = 8; // Not used
const int ci_tiltHomeHallPin = 11; 
const int ci_tiltDownHallPin = 9;
const int ci_tiltUpHallPin = 8;

//Declarations
Servo magServo;     // For firing arm servo on firing mech
ramp motorRamp;     // For dart flywheels on firing mech
AMS_5600 magSensor; // For magazines magnetic encoder
AccelStepper panStepper (motorInterfaceType, ci_panStepPin, ci_panDirPin);    // Pan axis on Pan/Tilt Assembly
AccelStepper tiltStepper (motorInterfaceType, ci_tiltStepPin, ci_tiltDirPin); // Tilt axis on Pan/Tilt Assembly
AccelStepper magStepper (motorInterfaceType, ci_magStepPin, ci_magDirPin);    // Stepper used to rotate magazine on firing mech

//Constants
const int ci_dartPos[12] = { 250 , 568, 938, 1275, 1609, 1973, 2293, 2616, 2949, 3332, 3668, 3991 };
const byte cb_servoIn = 130;
const byte cb_servoOut = 10;
const byte cb_vMaxPWM = 30;
const int ci_motorSpinUpPeriod = 5000;
const int ci_maxMagSpeed = 100;

//Variables
bool b_serialDebugPrint = false; // Enables serial debug text - Variable not currently used
bool b_panAxisHomed = false;     // Flag to confirm pan axis has been homed - Variable not currently used
bool b_tiltAxisHomed = false;    // Flag to confirm tilt axis has been homed - Variable not currently used
int i_stateMachine = 0;          // Store variable for state machine switch case - Variable not currently used
int i_tiltHomingDirection;       // 0 = was previously pointing up, 1 = was previously pointing down - Variable not currently used

int i_panTargetCoord = 0;  // Store the target coordinate for the pan axis - Variable not currently used
int i_tiltTargetCoord = 0; // Store the target coordinate for the tilt axis - Variable not currently used

int i_panHomeHallDetect;  // Stores coordinate for where panHomeHallPin was detected
int i_tiltHomeHallDetect; // Stores coordinate for calculated location of tiltHomeHallPin
int i_tiltDownHallDetect; // Stores coordinate for where tiltDownHallPin was detected
int i_tiltUpHallDetect;   // Stores coordiante for where tiltUpHallPin was detected

//dev
int i_panScanRange = 200;  // Used in 'scanning_func' to define target coordinate for pan axis
int i_tiltScanRange = 100; // Used in 'scanning_func' to define target coordinate for tilt axis
bool b_panScanDir = 0;     // Used in 'scanning_func' to keep track of whether pan axis has finished panning in one direction
bool b_tiltScanDir = 0;    // Used in 'scanning_func' to keep track of whether tilt axis has finished tilting in one direction

void setup() 
{
  //Comms Setup
  Serial.begin(74880);
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  Serial.println("Machine Vision Foam Dart Sentry");
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
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
  pinMode(ci_tiltHomeHallPin, INPUT);
  pinMode(ci_tiltDownHallPin, INPUT);
  pinMode(ci_tiltUpHallPin, INPUT);



  //Dev
  panStepper.setMaxSpeed(1000.0);    //This sets the maximum speed, datatype is float.
  panStepper.setAcceleration(25.0);  //This sets the acceleration and deceleration rates in steps/s, datatype is float.
  tiltStepper.setMaxSpeed(1000.0);   //This sets the maximum speed, datatype is float.
  tiltStepper.setAcceleration(25.0); //This sets the acceleration and deceleration rates in steps/s, datatype is float.

  homing_func(); // Homes the mechanism before entering the main program loop
}

//Loop Function
void loop() 
{
  scanning_func();
}


//---------------- Custom Functions ----------------\\

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
    if ( i_tmp > (ci_dartPos[dart] - 3) && i_tmp < (ci_dartPos[dart] + 3)) 
    {
      return 0;
    }
    else if ( i_tmp > ci_dartPos[dart]) 
    {
      magStepper.setSpeed(50); 
      magStepper.runSpeed();
    }
    else 
    {
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
  /*
    -- Homing Function --
    The code in this funciton will drive the pan/tilt head to its 'home' hall effect sensors.
    
    - Homing the tilt axis -
    (1) The tilt axis will first be poiting down as this is more secure for transit
    (2) Drive tilt axis up at a constant speed until the 'tiltUpHallPin' is detected 'low', then record current coordinate
    (3) Drive tilt axis down at constant speed until the 'tiltDownHallPin' is detected 'low', then record current coordinates
    (4) Calculate the midpoint between the coordinates for (2) and (3)
    (5) Drive tilt axis to calculated midpoint and stop tilt axis

    - Homing the pan axis -
    (1) Drive the pan axis around at a constant speed until 'panHomehallPin' is detected 'low' 
    (2) When 'panHomeHallPin' is detected 'low', stop the pan axis and record current coordinate

  */

  //Tilt Up till Limit 
  tiltStepper.setSpeed(-100);
  while(digitalRead(ci_tiltUpHallPin) == 1) // Hall effect output is low when magnet is detected
  {
    tiltStepper.runSpeed();
  }
  tiltStepper.stop();
  i_tiltUpHallDetect = tiltStepper.currentPosition();
  delay(200);

  //Tilt Down till Limit
  tiltStepper.setSpeed(100);
  while(digitalRead(ci_tiltDownHallPin) == 1)
  {
    tiltStepper.runSpeed();
  }
  tiltStepper.stop();
  i_tiltDownHallDetect = tiltStepper.currentPosition(); 
  delay(200);

  //Calculate mid then move to.
  if (i_tiltDownHallDetect > i_tiltUpHallDetect) 
  {
    i_tiltHomeHallDetect = ((i_tiltDownHallDetect - i_tiltUpHallDetect) / 2) + i_tiltUpHallDetect;
  }
  else 
  {
    i_tiltHomeHallDetect = ((i_tiltUpHallDetect - i_tiltDownHallDetect) / 2) + i_tiltDownHallDetect;
  }
  tiltStepper.moveTo(i_tiltHomeHallDetect);
  while(tiltStepper.currentPosition() != i_tiltHomeHallDetect)
  {
    tiltStepper.run();
  }
  tiltStepper.stop();
  // Possibly add 'i_tiltHomeHallDetect = tiltStepper.currentPosition(0);' here?
  // Parsing '0' into 'currentPosition' will set the current location as the Zero point for further coordinates which could be useful as.
  // Though this is possibly more relevant for simplifying 'tracking_func' logic, current implementation does work.
  delay(200);

  //Home Pan
  panStepper.setSpeed(100);
  while(digitalRead(ci_panHomeHallPin) == 1)
  {
    panStepper.runSpeed();
  }
  panStepper.stop();
  i_panHomeHallDetect = panStepper.currentPosition(); // If we parse '0' into currentPosition, the current location will be set as coordinate 0
  delay(200);
  
}// end of homing_func

void scanning_func() //Contains the function for 'scanning' for potential tartets
{
  /*
    -- Scanning Function --
    The code in this function drives pan and tilt axes between coordinates either side of their home positions.
    This will allow for the machine vision system to get a better view of potential targets infront of the system.

  */

  //Pan Stepper Control
  if(b_panScanDir == 0)
  {
    panStepper.moveTo(i_panHomeHallDetect - i_panScanRange);
  }
  else
  {
    panStepper.moveTo(i_panHomeHallDetect + i_panScanRange);
  }

  if(panStepper.distanceToGo() == 0) // 'distanceToGo' returns how many steps are left until the target value is met
  {
    b_panScanDir = !b_panScanDir;    // Setting this will cause axis to rotate in opposite direction
  }
    
  //Tilt Stepper Control
  if(b_tiltScanDir == 0)
  {
    tiltStepper.moveTo(i_tiltHomeHallDetect - i_tiltScanRange);
  }
  else
  {
    tiltStepper.moveTo(i_tiltHomeHallDetect + i_tiltScanRange);
  }

  if(tiltStepper.distanceToGo() == 0)
  {
    b_tiltScanDir = !b_tiltScanDir;
  }

  panStepper.run();
  tiltStepper.run();
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
