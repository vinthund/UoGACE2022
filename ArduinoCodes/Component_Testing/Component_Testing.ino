#include <Servo.h>
#include <Ramp.h>
#include <Wire.h>
#include <AS5600.h>

#define servo_pin 10
#define motors_pin 5
#define stepper 3
#define stepper_pos A5
//#define button 13

#define vmaxPWM 30

Servo servo;
ramp motorRamp;
AMS_5600 ams5600;

byte out = 10;
byte in = 130;

void setup() {
  //Comms Setup
  Serial.begin(115200);
  Wire.begin();

  //Servo Pin Setup
  servo.attach(servo_pin);

  //Stepper Pin Setup
  pinMode(stepper, OUTPUT);

  //Stepper Position Setup
  pinMode(stepper_pos, INPUT);

  //Firing Motor Pin Setup
  pinMode(motors_pin, OUTPUT);
  digitalWrite(motors_pin, LOW);

  //Firing Motor Control Setup
  delay(2000);
  motorRamp.go(vmaxPWM, 5000);

  for (int i = 0; i < 4000; i++)
  {
    //Spinning Stepper Motor
    digitalWrite(stepper, LOW);
    delayMicroseconds(1000);
    digitalWrite(stepper,HIGH);
    delayMicroseconds(1000);

    //Stepper Motor Position Monitor
    Serial.println(String(convertRawAngleToDegrees(ams5600.getRawAngle()),DEC));
  
  }
}

void loop() {

  //Firing Motor Control Example
  //analogWrite(motors_pin, motorRamp.update());

  
 
}

//Convert Raw output from AS5600 to Angle in Degrees
float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087 of a degree */
  float retVal = newAngle * 0.087;
  return retVal;
}
