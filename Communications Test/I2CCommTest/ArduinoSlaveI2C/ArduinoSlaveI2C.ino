/* I2C Communication Code
    Tested between Arduino and its Master, (Currently Raspberry Pi).
    Communication contents will consist of status codes to update the Raspberry Pi of its current status, and vice versa.
    SDA <--> SDA
    SCL <--> SCL
    GND <--> GND

    Version 2.1.7
   Changes made:
        Addition of coordinate variables - MM
        Removal of H Bridge outputs - MM
        Addition made to statusCodes function such that it detects data sent from Pi and translates them accordingly. - M
        Addition of new line which handles receiving and storing coordinates, this is done through "sscanf" which assigns the numbers within the coordinates to variable x and y - M
        Addition of new start up function. - MM
        Addition of conversion function which only prints numbers and coordinates barring texts. - MM
        Removed problematic else statement from statusCodes function which would flood serial monitor with "unknown" - MM
        Removed led pin13 due to being unused - MM
        Addition of new boolean variables "trigger" and "movement", both responsible for activating when the process of shooting and movement happens respectively. - M
        (continuation) each is to be used as a switch so that they do not shoot and move at the same time.
        Addition of new variable "ammo" to represent amount of ammo left. - M
        Addition of Servo library and code - M
*/

// Include the Wire library for I2C
#include <Wire.h>
#include <Servo.h>

Servo myservo;
// Definition of H Bridge, LED pins and integers, note that these are here for test purposes and may be controlled using I2C.
String received_str = "";
int toSend = 1;
int x, y, ammo, mapper, x_old, y_old;
int pos, ypos = 0;
bool flag, constant, trigger, movement = false;

void setup() {
  myservo.attach(2);
  // Join I2C bus as slave with address 8
  Wire.begin(0x8);

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
  if (received_str == "1") {
    Serial.println("Raspberry Pi - Connection Established");
  } else if (received_str == "2") {
    Serial.println("Raspberry Pi - Data Received");
  } else if (received_str == "100") {
    Serial.println("Raspberry Pi - Status OK");
  } else if (received_str[0] == '(' ) {
    Serial.println("Coordinates Received, Ready to Delete Target!");
    if (constant == true) {
      movement = false;
    } else {
      movement == true;
    }
    int r = sscanf(received_str.c_str(), "(%d, %d)", &x, &y);
    Serial.print("X: ");
    Serial.println(x);
    Serial.print("Y: ");
    Serial.println(y);
  }
}
// Function that deals with startUp, establishing connection between the Pi and the Arduino.
void startUp() {
  if (flag == false) {
    toSend = 1;
    delay(3000); // delay of 3 seconds before setting flag to true
    flag = true;
  }
}

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