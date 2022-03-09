/* I2C Communication Code
    Tested between Arduino and its Master, (Currently Raspberry Pi).
    Communication contents will consist of status codes to update the Raspberry Pi of its current status, and vice versa.
    SDA <--> SDA
    SCL <--> SCL
    GND <--> GND

    Version: 1.6.4

    Changes made:
    * Addition of coordinate variables
    * Removal of H Bridge outputs
    * Addition made to statusCodes function such that it detects data sent from Pi and translates them accordingly.
    * Addition of test code to receive coordinates
*/

// Include the Wire library for I2C
#include <Wire.h>

// Definition of H Bridge, LED pins and integers, note that these are here for test purposes and may be controlled using I2C.
#define ledPin 13
String received_str = "";
bool flag = false;
int toSend = 1;
int toSendtest = 0;
int x_coord = 0;
int y_coord = 0;
int z_coord = 0;


void setup() {
  // Join I2C bus as slave with address 8
  Wire.begin(0x8);

  // Call receiveEvent when data received and requestEvent when data is to be sent
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  //pinMode definitons for H bridge to be possibly used for later.
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
  if (receivedMsg == "1") {
    Serial.println("Raspberry Pi - Connection Established");
  } else if (receivedMsg == "2") {
    Serial.println("Raspberry Pi - Data Received");
  } else if (receivedMsg == "100") {
    Serial.println("Raspberry Pi - Status OK");
  } else if (receivedMsg.substring(0) == "(" ){
   //do some code that splits the string intochunks of xyz with the comma acting as a delimiter
  } else {
    Serial.println("Raspberry Pi - Status Unknown");
  }
}

void loop() {
  delay(100);
  
  if (flag == false) {
    delay(2500);
    toSend=1;
    flag = true;
  }
  toSend = 100;
  statusCodes();

  

  int n = received_str.toInt();
  if (n > 0) { //Data Conversion utilised in a sense that when n is at 0, meaning that it is a character, does not get outputted, else print the number and reset.
    Serial.println(received_str);
    received_str = "";
  } else {
    Serial.println(received_str);
    received_str = "";
  }

}
