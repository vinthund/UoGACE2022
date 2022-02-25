#include <Servo.h>
#include <Ramp.h>
#include <Wire.h>
#include <AS5600.h>

#define servo_pin 10
#define motors_pin 5
#define stepper 2
#define stepper_pos A5
//#define button 13

#define vmaxPWM 30

Servo servo;
ramp motorRamp;
AMS_5600 ams5600;

byte out = 10;
byte in = 130;

int dart[12] = { 18 , 48, 78, 108, 138, 168, 198, 228, 258, 288, 318, 348 };

void setup() {
  //Comms Setup
  Serial.begin(74880);
  Serial.println("HELLO");
  Wire.begin();

  //Servo Pin Setup
  servo.attach(servo_pin);

  //Stepper Pin Setup
  pinMode(stepper, OUTPUT);

  //Stepper Position Setup
  pinMode(stepper_pos, INPUT);

  //Firing Motor Pin Setup
  pinMode(motors_pin, OUTPUT);
  //digitalWrite(motors_pin, LOW);

  //Firing Motor Control Setup
  delay(5000);
  motorRamp.go(vmaxPWM, 5000);

}

void loop() {

  //Firing Motor Control Example
  //analogWrite(motors_pin, motorRamp.update());

  //Spinning Stepper Motor
  //digitalWrite(stepper, LOW);
  //delayMicroseconds(1000);
  //digitalWrite(stepper,HIGH);
  //delayMicroseconds(1000);
    
  //Stepper Motor Position Monitor
  Serial.println(String(convertRawAngleToDegrees(ams5600.getRawAngle()),DEC));
  delay(20);

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
}
