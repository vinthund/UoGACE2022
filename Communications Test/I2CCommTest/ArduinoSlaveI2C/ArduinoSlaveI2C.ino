/*This code was taken and modified from my (Joel) Individual project. This code basically manipulates an H bridge whenever receiving commands from Python, flipping polarity depending on the type
 * of command given e.g. "positive", "negative" and "off". This is the I2C Version of the UART code made earlier.
 * SDA <--> SDA
 * SCL <--> SCL
 * GND <--> GND
 * Version: 1
*/

// Include the Wire library for I2C
#include <Wire.h>

#define ENA 2
#define IN1 3
#define IN2 4
#define ledpin 13

 
void setup() {
  // Join I2C bus as slave with address 8
  Wire.begin(0x8);
  
  // Call receiveEvent when data received                
  Wire.onReceive(receiveEvent);
  
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
  Serial.begin(9600);
}
 
// Function that executes whenever data is received from master
void receiveEvent(int howMany) {
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    digitalWrite(ledPin, c);
  }
}
void loop() {
  delay(100);
}
