#include <AccelStepper.h>
#include <Servo.h>
#include <Wire.h>
#include <AS5600.h>
#include <Ramp.h>

//Definitions
#define motorInterfaceType 1

//Stepper pins
const int ci_magStepPin = 2;
const int ci_magDirPin = A2;

//Servo Pins
const int ci_servoPin = 10;

//Motor Pins
const int ci_motorPin = 5;

//Declare objects of AccelStepper to handle pan and tilt axes, and magazine rotation
AccelStepper magStepper (motorInterfaceType, ci_magStepPin, ci_magDirPin);

//Declare Servo objects
Servo magServo;

//Declare RAMP objects
ramp motorRamp;

//Declare AS5600 Magentic Position Sensor
AMS_5600 magSensor;

//Const Variables
const int ci_dartPos[12] = { 250 , 568, 938, 1275, 1609, 1973, 2293, 2616, 2949, 3332, 3668, 3991 };
const byte cb_servoIn = 130;
const byte cb_servoOut = 10;
const byte cb_vMaxPWM = 30;
const int ci_motorSpinUpPeriod = 5000;
const int ci_maxMagSpeed = 100;

void setup()
{
  Serial.begin(115200);
  Serial.println(">>>>>>>>>>>>>>>>>>>");
  Wire.begin();

  //Firing Motor Setup
  digitalWrite(ci_motorPin, 0);
   
  //Output pins
  pinMode(ci_magStepPin, OUTPUT);
  pinMode(ci_magDirPin, OUTPUT);
  pinMode(ci_motorPin, OUTPUT);

  //Servo Pin Setup
  magServo.attach(ci_servoPin);
  magServo.write(cb_servoOut);

  //Stepper Setups
  magStepper.setMaxSpeed(ci_maxMagSpeed);


  //Example Sketch
  //Homes magazine, spins up motors, fires each dart, spinds down motors.
  home_mag();
  spinUp();
  for ( byte d = 0; d < 12; d++ ) {
    goToDart(d);
    fire();
  }
  spinDown();
}

void loop()
{
  
} //End of 'loop'


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
