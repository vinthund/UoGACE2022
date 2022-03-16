#include <Servo.h>
#include <Ramp.h>
#include <Wire.h>
#include <AS5600.h>

#define servo_pin 10
#define motors_pin 5
#define mag_step 2
#define stepper_pos A5
#define pan_dir 3
#define pan_step 4
//#define button 13

#define vmaxPWM 30

Servo servo;
ramp motorRamp;
AMS_5600 ams5600;

byte out = 10;
byte in = 130;

bool dir = 1;

int dart[12] = { 18 , 48, 78, 108, 138, 168, 198, 228, 258, 288, 318, 348 };
//dev
int i_panScanRange = 200;
int i_tiltScanRange = 100;
bool b_panScanDir = 0;
bool b_tiltScanDir = 0;


void setup() {
  //Comms Setup
  Serial.begin(74880);
  Serial.println("HELLO");
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  Serial.println("Machine Vision Foam Dart Sentry");
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  Wire.begin();

  //Servo Pin Setup
  servo.attach(servo_pin);

  //Stepper Pin Setup
  pinMode(stepper, OUTPUT);

  //Pan Stepper Setup
  pinMode(pan_dir, OUTPUT);
  pinMode(pan_step, OUTPUT);

  //Tilt Stepper Setup
  //pinMode(tilt_dir, OUTPUT);
  //pinMode(tilt_step, OUTPUT);

  //Stepper Position Setup
  pinMode(stepper_pos, INPUT);

  //Firing Motor Pin Setup
  pinMode(motors_pin, OUTPUT);
  //digitalWrite(motors_pin, LOW);

  //Firing Motor Control Setup
  delay(5000);
  motorRamp.go(vmaxPWM, 5000);

  //Dev
  panStepper.setMaxSpeed(1000.0); //This sets the maximum speed, datatype is float.
  panStepper.setAcceleration(25.0); //This sets the acceleration and deceleration rates in steps/s, datatype is float.
  tiltStepper.setMaxSpeed(1000.0); //This sets the maximum speed, datatype is float.
  tiltStepper.setAcceleration(25.0); //This sets the acceleration and deceleration rates in steps/s, datatype is float.

  homing_func();
  
}

void loop() {

  //Firing Motor Control Example
  //analogWrite(motors_pin, motorRamp.update());
  scanning_func();
}


//---------------- Custom Functions ----------------\\

void home_mag() //Contains the function for homing the magazine to dart zero
{
   goToDart(0);
}

  //Spinning Stepper Motor
  if(Serial.available()) {
    if(Serial.read() == 'a') {
      dir = 1;
      Serial.read();
    }
    else {
      dir = 0;
    }
    Serial.println(dir);
  }
  digitalWrite(pan_dir, dir);
  digitalWrite(pan_step, LOW);
  delayMicroseconds(1000);
  digitalWrite(pan_step,HIGH);
  delayMicroseconds(1000);
    
  //Stepper Motor Position Monitor
  //Serial.println(String(convertRawAngleToDegrees(ams5600.getRawAngle()),DEC));
  //delay(20);

  //Servo Control
  //Serial.println(in);
  //digitalWrite(LED_BUILTIN,HIGH);
  //servo.write(in);
  //delay(2000);
  //digitalWrite(LED_BUILTIN,LOW);
  //Serial.println(out);
  //servo.write(out);
  //delay(2000);
 
}

//Convert Raw output from AS5600 to Angle in Degrees
float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087 of a degree */
  float retVal = newAngle * 0.087;
  return retVal;
  //Tilt Up till Limit
  tiltStepper.setSpeed(-100);
  while(digitalRead(ci_tiltUpHallPin) == 1){
    tiltStepper.runSpeed();
  }
  tiltStepper.stop();
  i_tiltUpHallDetect = tiltStepper.currentPosition();
  delay(200);

  //Tilt Down till Limit
  tiltStepper.setSpeed(100);
  while(digitalRead(ci_tiltDownHallPin) == 1){
    tiltStepper.runSpeed();
  }
  tiltStepper.stop();
  i_tiltDownHallDetect = tiltStepper.currentPosition();
  delay(200);

  //Calculate mid then move to.
  if (i_tiltDownHallDetect > i_tiltUpHallDetect) {
    i_tiltHomeHallDetect = ((i_tiltDownHallDetect - i_tiltUpHallDetect) / 2) + i_tiltUpHallDetect;
  }
  else {
    i_tiltHomeHallDetect = ((i_tiltUpHallDetect - i_tiltDownHallDetect) / 2) + i_tiltDownHallDetect;
  }
  tiltStepper.moveTo(i_tiltHomeHallDetect);
  while(tiltStepper.currentPosition() != i_tiltHomeHallDetect){
    tiltStepper.run();
  }
  tiltStepper.stop();
  delay(200);

  //Home Pan
  panStepper.setSpeed(100);
  while(digitalRead(ci_panHomeHallPin) == 1){
    panStepper.runSpeed();
  }
  panStepper.stop();
  i_panHomeHallDetect = panStepper.currentPosition();
  delay(200);
  
}// end of homing_func

void scanning_func() //Contains the function for 'scanning' for potential tartets
{
  //Pan Stepper Control
  if(b_panScanDir == 0){
    panStepper.moveTo(i_panHomeHallDetect - i_panScanRange);
  }
  else{
    panStepper.moveTo(i_panHomeHallDetect + i_panScanRange);
  }

  if(panStepper.distanceToGo() == 0){
    b_panScanDir = !b_panScanDir;
  }
    
  //Tilt Stepper Control
  if(b_tiltScanDir == 0){
    tiltStepper.moveTo(i_tiltHomeHallDetect - i_tiltScanRange);
  }
  else{
    tiltStepper.moveTo(i_tiltHomeHallDetect + i_tiltScanRange);
  }

  if(tiltStepper.distanceToGo() == 0){
    b_tiltScanDir = !b_tiltScanDir;
  }

  panStepper.run();
  tiltStepper.run();
}
