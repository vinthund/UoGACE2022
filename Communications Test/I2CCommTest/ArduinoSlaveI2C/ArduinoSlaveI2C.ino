/* I2C Communication Code
    Tested between Arduino and its Master, (Currently Raspberry Pi).
    Communication contents will consist of status codes to update the Raspberry Pi of its current status, and vice versa.
    SDA <--> SDA
    SCL <--> SCL
    GND <--> GND

    Version 2.3.1
   Changes made:
     * Addition of ammo checking condition that checks if ammo is within container. If there is, it triggers the shoot flag, thereby shooting the target with foam.
*/

// Include the Wire library for I2C
#include <Wire.h>
#include <Servo.h>

Servo myservo;
Servo myservo2;
String received_str = "";
int toSend;
int x, y, ammo;
bool flag, trigger, movement = false;

void setup() {
  myservo.attach(2);
  myservo2.attach(3);
  // Join I2C bus as slave with address 8
  Wire.begin(0x8);

  // Call receiveEvent when data received and requestEvent when data is to be sent
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.begin(9600);
  //Connection establishment: Sets "toSend" to 1 which means "Connection Established". This then sets the flag to true so that it does not set "toSend" to 1 again.
  toSend = 1;
  delay(3000);
  flag = true;
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
  //Static keyword used to create variable for only one function. This, unlike other variables, keeps its value after function calls, allowing it to
  // be compared with newer versions of values to make comparisons with them. This is used in this code to avoid the servo moving over to the same position again
  // even though it is receiving the same coordinates.
  static int x_old = 0;
  static int y_old = 0;

  if (received_str == "1") {
    Serial.println("Raspberry Pi - Connection Established");
  } else if (received_str == "2") {
    Serial.println("Raspberry Pi - Data Received");
  } else if (received_str == "100") {
    Serial.println("Raspberry Pi - Status OK");
  } else if (received_str[0] == '(' ) {
    Serial.println("Coordinates Received, Ready to Delete Target!");
    //sscanf is used here to read formatted input from a string, then assigns the values to be read onto integer varianbles.
    // in this case, they are set to coordinate "x" and "y" variables. 
    int r = sscanf(received_str.c_str(), "(%d, %d)", &x, &y);
    Serial.print("X: ");
    Serial.println(x);
    Serial.print("Y: ");
    Serial.println(y);
    // Below conditional statement checks if the current coordinate variables are equal to its previous versions stored on the static int variables.
    // In the case it is, the movement flag is set to false, blocking movement from occuring as no change has occured.
    // However, in the case that it is not the same as its previous version, movement is set to true, then the coordinate variables are stored in the static int variables,
    // so as to be compared to the next values sent.
    if (x == x_old && y == y_old) {
      movement = false;
    } else {
      movement = true;
      x_old = x;
      y_old = y;
    }
  }
}

//The Below function works on blocking strings/characters from appearing except brackets and numbers.
void convertToInt() {
  int n = received_str.toInt();
  if (n > 0 || received_str[0] == '(') {
    //Serial.println(received_str);
    received_str = "";
  } else {
    received_str = "";
  }
}

/* The below function handles target detection.
 * It first maps the values from 0 to 1023(current limit) to 0 to 180. This is done due the servo being limited at ~180 degrees.
 * While it is true that it could instead be done from -90 to 90, doing this does not work, as it limits the servo for some reason into just 90 degrees.
 * The conditional statement within the function first checks if movement is true. As you remember, movement was the flag within the 
 * statusCodes function to check if the values being received were the same or were different. If they were different, it would set "movement" to true.
 * Thus, the below statement checks if this flag is true, then writes on to both servo "x" and "y" the values that were sent over I2C. 
 * After it has finished writing the values, it sets movement to false to block any further movements.
 */
void onDetectTarget() {
  x = map(x, 0, 1023, 0, 180);
  y = map(y, 0, 1023, 0, 180);
  if (movement == true) {
    myservo.write(x);
    myservo2.write(y);
    /* Responsible for handling shooting
    if (ammo > 3) {
      trigger = true;
      //start shooting
    } else {
      trigger = false;
    }
    */
    delay(15);
    movement = false;
  }
}

//Functions from above all being run in loop()
void loop() {
  delay(100);
  toSend = 100;
  statusCodes();
  convertToInt();
  onDetectTarget();
}