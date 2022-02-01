#include <Servo.h>
#include <Ramp.h>

#define servo_pin 10
#define motors_pin 5
#define stepper 3
//#define button 13

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
  digitalWrite(motors_pin, HIGH);

  //Enable Fast PWM
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20);

  motorRamp.go(80, 5000);
}

void loop() {

  analogWrite(motors_pin, 200);

  //Spinning Stepper Motor
  /*digitalWrite(stepper, LOW);
  delay(1);
  digitalWrite(stepper,HIGH);
  delay(1);*/
  
 
}
