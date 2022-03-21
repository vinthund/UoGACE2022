/* UART Communications Code
    Tested between Arduino and its Master, (Currently Raspberry Pi).
    Communication contents will consist of status codes to update the Raspberry Pi of its current status, and vice versa.
    This is an alternate version of its I2C Counterpart. This code was created due to the Arduino already using I2C to communicate with another device for the gun.
    Thus, another serial communication option had to be selected to make communication work.

    Note that this code is a test environment that assesses the viability of communication between a hypothetical Jetson Nano
    or the Pi in this case, and the Arduino. So whilst communication will be tested here, things like firing and servo motor control
    are there as examples to test if mechanical equipment will operate under commands being sent over serial, and to see if they are fast
    enough to react to incoming information. It is important that it is fast enough, as in a real world situation, the target will
    try to "dodge" the projectiles hence the coordinates constantly changing and streaming said coordinates to move then shoot
    at the target.

    Pi <--> Arduino
    Rx <--> Tx
    Tx <--> Rx
    GND <--> GND

    Version 0.7.3
   Changes made:
       * Added new buffer variable that stores the Ammo and its value which is then sent over Serial to the Pi. It lets the Pi know the amount of bullets remaining when the turret is active.
       * Addition of new for loop that simulates how the turret is run, this is used to send values ranging from 12 to 0 to the Pi.
       * Added "reactions" to data being sent by the Pi, just to confirm to the Pi that the Arduino is receiving the information being sent.
       * Removed list of Serial prints and replaced it with snprintf and one Serial Print, which concatenates the string into a char buffer nicely then prints it over serial respectively.
       * Added preprocessing directives DEBUG to test out some parts of the code.

*/

// Include the SoftwareSerial library for UART
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>

SoftwareSerial mySerial(2, 3); // RX, TX
Servo myservo, myservo2;
String received_str = "";
int toSend;
int x, y, ammo;
bool flag, trigger, movement = false;
char out[50];
char buffer[90];


void setup() {
  myservo.attach(4);
  myservo2.attach(5);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  mySerial.begin(9600);
  //Connection establishment: Sets "toSend" to 1 which means "Connection Established". This then sets the flag to true so that it does not set "toSend" to 1 again.
  toSend = 1;
  delay(3000);
  flag = true;
}

// Function that sends data to Master (Pi). Unlike receiveEvent, the Wire.available confirmation loop cannot be used as it stops data being sent.
void requestEvent() {
  mySerial.println(toSend);
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
    toSend = 101;
    Serial.println("Raspberry Pi - Data Received");
  } else if (received_str == "100") {
    Serial.println("Raspberry Pi - Status OK");
    toSend = 101;
  } else if (received_str[0] == '(' ) {
    Serial.println("Coordinates Received, Ready to Delete Target!");
    toSend = 101;
    //sscanf is used here to read formatted input from a string, then assigns the values to be read onto integer varianbles.
    // in this case, they are set to coordinate "x" and "y" variables.
    int r = sscanf(received_str.c_str(), "(%d, %d)", &x, &y);
    snprintf(out, 30, "X: %d\nY: %d", x, y);
    Serial.print(out);
    // Below conditional statement checks if the current coordinate variables are equal to its previous versions stored on the static int variables.
    // In the case it is, the movement flag is set to false, blocking movement from occuring as no change has occured.
    // However, in the case that it is not the same as its previous version, movement is set to true, then the coordinate variables are stored in the static int variables,
    // so as to be compared to the next values sent.

    if (x == x_old && y == y_old) {
      movement = false;
      for (ammo = 12; ammo >= 0; ammo--) {
        snprintf( buffer, 90, "Ammo: %d", ammo );
        mySerial.println( buffer );
        delay(500);
      }
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
   It first maps the values from 0 to 1023(current limit) to 0 to 180. This is done due the servo being limited at ~180 degrees.
   While it is true that it could instead be done from -90 to 90, doing this does not work, as it limits the servo for some reason into just 90 degrees.
   The conditional statement within the function first checks if movement is true. As you remember, movement was the flag within the
   statusCodes function to check if the values being received were the same or were different. If they were different, it would set "movement" to true.
   Thus, the below statement checks if this flag is true, then writes on to both servo "x" and "y" the values that were sent over UART.
   After it has finished writing the values, it sets movement to false to block any further movements.
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
  requestEvent();
  //Below code deals with reading incoming strings being sent over serial. Essentially, it reads the string "until" there is the the spacing
  // '\n', it will assign whatever is being sent in the variable "received_str". This variable then gets processed in the convertToInt function.
  received_str = mySerial.readStringUntil('\n');
#ifdef DEBUG
  //toSend = 100;
  //Serial.println(toSend);
#endif
  statusCodes();
  convertToInt();
  onDetectTarget();
}
