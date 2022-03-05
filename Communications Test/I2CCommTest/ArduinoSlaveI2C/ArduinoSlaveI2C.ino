/* I2C Communication Code
    Tested between Arduino and its Master, (Currently Raspberry Pi).
    Communication contents will consist of status codes to update the Raspberry Pi of its current status, and vice versa.
    SDA <--> SDA
    SCL <--> SCL
    GND <--> GND

    Version: 1.6.1

    Changes made:
    * RequestEvent function now able to send any kind of message to Pi with only a variable and without using requestEvent's parameters
    * Implementation of a Heartbeat Signal
    * Implementation of a "Connection Established" message when first starting program
    * Status Codes addition.
*/

// Include the Wire library for I2C
#include <Wire.h>

// Definition of H Bridge, LED pins and integers, note that these are here for test purposes and may be controlled using I2C.
#define ENA 2
#define IN1 3
#define IN2 4
#define ledPin 13
String received_str = "";
bool flag = false;
int toSend = 1;
int toSendtest = 0;


void setup() {
  // Join I2C bus as slave with address 8
  Wire.begin(0x8);

  // Call receiveEvent when data received and requestEvent when data is to be sent
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  //pinMode definitons for H bridge to be possibly used for later.
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  Serial.begin(9600);
}

// Function that executes whenever data is received from master
void receiveEvent(int howMany) {
  while (Wire.available()) { // loop through all but the last
    // receive byte as a character, char converts unicode to character and add these characters into string
    received_str += (char)Wire.read();
  }
}

// Function that sends data to Master (Pi). Unlike receiveEvent, the Wire.available confirmation loop cannot be used as it stops data being sent.
void requestEvent() {
  Wire.write(toSend);
}

// Function that deals with status codes to control Arduino
void statusCodes() {
  String receivedMsg = received_str;
  if (receivedMsg == "100") {
    Serial.println("Status OK");
  }
}

void loop() {
  delay(100);
  
  if (flag == false) {
    Serial.print(Wire.available());
    delay(5000);
    toSend=1;
    flag = true;
  }
  toSend = 100;
  statusCodes();
  toSendtest++;
  if (toSendtest >= 10) {
    toSendtest = 0;
  }
  

  int n = received_str.toInt();
  if (n > 0) { //Data Conversion utilised in a sense that when n is at 0, meaning that it is a character, does not get outputted, else print the number and reset.
    Serial.println(received_str);
    received_str = "";
  } else {
    received_str = "";
  }

}