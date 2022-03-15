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
#define tilt_dir A1
#define tilt_step A0
//#define button 13

#define vmaxPWM 30

Servo servo;
ramp motorRamp;
AMS_5600 ams5600;

byte out = 10;
byte in = 130;

bool dir = 1;

int dart[12] = { 18 , 48, 78, 108, 138, 168, 198, 228, 258, 288, 318, 348 };

void setup() {
  //Comms Setup
  Serial.begin(74880);
  Serial.println("HELLO");
  Wire.begin();

  //Servo Pin Setup
  servo.attach(servo_pin);

  //Stepper Pin Setup
  pinMode(mag_step, OUTPUT);

  //Pan Stepper Setup
  pinMode(pan_dir, OUTPUT);
  pinMode(pan_step, OUTPUT);

  //Tilt Stepper Setup
  pinMode(tilt_dir, OUTPUT);
  pinMode(tilt_step, OUTPUT);

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
  
  digitalWrite(tilt_dir, dir);
  digitalWrite(tilt_step, LOW);
  delayMicroseconds(1000);
  digitalWrite(tilt_step,HIGH);
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
}
