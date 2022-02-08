#include <Servo.h>
#include <Ramp.h>

#define servo_pin 10
#define motors_pin 5
#define stepper 3
//#define button 13

#define vmaxPWM 30

Servo servo;
ramp motorRamp;

byte out = 10;
byte in = 130;

int volts = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  servo.attach(servo_pin);

  pinMode(stepper, OUTPUT);
  
  pinMode(motors_pin, OUTPUT);
  digitalWrite(motors_pin, LOW);

  delay(2000);
  motorRamp.go(vmaxPWM, 5000);
}

void loop() {

  analogWrite(motors_pin, motorRamp.update());

  //Spinning Stepper Motor
  /*digitalWrite(stepper, LOW);
  delay(1);
  digitalWrite(stepper,HIGH);
  delay(1);*/
  
 
}
