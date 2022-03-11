/* I2C Communication Code
    Tested between Arduino and its Master, (Currently Raspberry Pi).
    Communication contents will consist of status codes to update the Raspberry Pi of its current status, and vice versa.
    SDA <--> SDA
    SCL <--> SCL
    GND <--> GND

    Version 2.1.9
   Changes made:
   * Removal of startup function, simplifying it into just "toSend=1" on "void setup()"
   * Addition of static bytes to store old values of positions received.
*/

// Include the Wire library for I2C
#include <Wire.h>
#include <Servo.h>

Servo myservo;
// Definition of H Bridge, LED pins and integers, note that these are here for test purposes and may be controlled using I2C.
String received_str = "";
int toSend = 1;
int x, y, ammo, mapper;
int pos, ypos = 0;
bool flag, constant, trigger, movement = false;

void setup() {
  myservo.attach(2);
  // Join I2C bus as slave with address 8
  Wire.begin(0x8);
  toSend = 1;
  delay(3000);
  flag = true;
  // Call receiveEvent when data received and requestEvent when data is to be sent
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

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
  static byte x_old = 0;
  static byte y_old = 0;

  if (received_str == "1") {
    Serial.println("Raspberry Pi - Connection Established");
  } else if (received_str == "2") {
    Serial.println("Raspberry Pi - Data Received");
  } else if (received_str == "100") {
    Serial.println("Raspberry Pi - Status OK");
  } else if (received_str[0] == '(' ) {
    Serial.println("Coordinates Received, Ready to Delete Target!");
    int r = sscanf(received_str.c_str(), "(%d, %d)", &x, &y);
    Serial.print("X: ");
    Serial.println(x);
    Serial.print("Y: ");
    Serial.println(y);
    if (x == x_old && y == y_old) {
      movement = false;
    } else {
      movement == true;
      x_old = x;
      y_old = y;
    }
  }
}
/*
// Function that deals with startUp, establishing connection between the Pi and the Arduino.
void startUp() {
  if (flag == false) {
    toSend = 1;
    delay(3000); // delay of 3 seconds before setting flag to true
    flag = true;
  }
}
*/
void convertToInt() {
  int n = received_str.toInt();
  if (n > 0 || received_str[0] == '(') {
    //Serial.println(received_str);
    received_str = "";
  } else {
    received_str = "";
  }
}

void onDetectTarget() {
  x = map(x, 0, 1023, 0, 180);
  y = map(y, 0, 1023, 0, 180);
  if (movement == true) {
    for (pos = 0; pos <= x; pos += 1) {
      myservo.write(pos);
      delay(15);
    }
    movement = false;
    //pos = 0;
    //Code to first map the values of xyz into limit of 255, then have the turret move to target and shoot
  }
}

void compareOldNew() {
  x_old = x;
  y_old = y;
  delay(900);
  if (x == x_old && y == y_old) {
    constant = true;
  } else {
    constant = false;
  }
}

void loop() {
  delay(100);
  startUp();
  toSend = 100;
  statusCodes();
  convertToInt();
  onDetectTarget();
  compareOldNew();
}