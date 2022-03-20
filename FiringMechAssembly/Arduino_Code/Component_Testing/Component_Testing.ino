#include <SoftwareSerial.h> // Standard Arduino library for bit banging serial data comms
#include <Servo.h>          // Standard Arduino library for controlling servos
#include <Ramp.h>           // Standard Arduino library used to interpolate between values
#include <Wire.h>           // Standard Arduino I2C library
#include <AS5600.h>         // AS5600 library can be found here: https://github.com/Seeed-Studio/Seeed_Arduino_AS5600 
#include <AccelStepper.h>   // AccelStepper library can be found here: https://github.com/waspinator/AccelStepper
                            // AccelStepper's class references can be found herehttps://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#details 

//Definitions
#define motorInterfaceType 1 // As stepper drivers are being used, this will configure AccelStepper to just send step pulses on a single output pin
#define vmaxPWM 30

//Stepper pins
const int ci_magStepPin = 2; // Defines the step pulse output pin
const int ci_magDirPin = A2; // Defines the direction output pin, low = move CCW, high = move CW

const int ci_panStepPin = 4;
const int ci_panDirPin = 3;
const int ci_tiltStepPin = A0;
const int ci_tiltDirPin = A1;

//Servo Pins - For firing mech
const int ci_servoPin = 10;

//Motor Pins - For firing mech
const int ci_motorPin = 5;

//Hall effect pins
const int ci_panHomeHallPin = 6;
//const int ci_panLeftHallPin = 7; // Not used
//const int ci_panRghtHallPin = 8; // Not used
const int ci_tiltHomeHallPin = 11; 
const int ci_tiltDownHallPin = 9;
const int ci_tiltUpHallPin = 8;

//Declarations
SoftwareSerial jSerial(A6, A7);   // Rx, Tx, Serial to Jetson
Servo magServo;                   // For firing arm servo on firing mech
ramp motorRamp;                   // For dart flywheels on firing mech
AMS_5600 magSensor;               // For magazines magnetic encoder
AccelStepper panStepper (motorInterfaceType, ci_panStepPin, ci_panDirPin);    // Pan axis on Pan/Tilt Assembly
AccelStepper tiltStepper (motorInterfaceType, ci_tiltStepPin, ci_tiltDirPin); // Tilt axis on Pan/Tilt Assembly
AccelStepper magStepper (motorInterfaceType, ci_magStepPin, ci_magDirPin);    // Stepper used to rotate magazine on firing mech

//Constants
const int ci_dartPos[12] = { 250 , 568, 938, 1275, 1609, 1973, 2293, 2616, 2949, 3332, 3668, 3991 };
const byte cb_servoIn = 130;
const byte cb_servoOut = 10;
const byte cb_vMaxPWM = 30;
const int ci_motorSpinUpPeriod = 5000;
const int ci_maxMagSpeed = 100;

//Variables
bool b_serialDebugPrint = false; // Enables serial debug text - Variable not currently used
bool b_panAxisHomed = false;     // Flag to confirm pan axis has been homed - Variable not currently used
bool b_tiltAxisHomed = false;    // Flag to confirm tilt axis has been homed - Variable not currently used
int i_stateMachine = 0;          // Store variable for state machine switch case - Variable not currently used
int i_tiltHomingDirection;       // 0 = was previously pointing up, 1 = was previously pointing down - Variable not currently used

int i_panTargetCoord = 0;  // Store the target coordinate for the pan axis - Variable not currently used
int i_tiltTargetCoord = 0; // Store the target coordinate for the tilt axis - Variable not currently used

int i_panHomeHallDetect;   // Stores coordinate for where panHomeHallPin was detected
int i_tiltHomeHallDetect;  // Stores coordinate for calculated location of tiltHomeHallPin
int i_tiltDownHallDetect;  // Stores coordinate for where tiltDownHallPin was detected
int i_tiltUpHallDetect;    // Stores coordiante for where tiltUpHallPin was detected

int i_panScanRange = 200;  // Used in 'scanning_func' to define range of motion for pan axis
int i_tiltScanRange = 100; // Used in 'scanning_func' to define range of motion for tilt axis
bool b_panScanDir = 0;     // Used in 'scanning_func' to keep track of whether pan axis has finished panning in one direction
bool b_tiltScanDir = 0;    // Used in 'scanning_func' to keep track of whether tilt axis has finished tilting in one direction

String s_received = "";    // Stores string recieved via jetson serial comms
int i_toSend;              // Stores integars to be sent to jetson via serial comms
int i_x;                   // Stores X co-ordinate passed by jetson via serial comms
int i_y;                   // Stores Y co-ordinate passed by jetson via serial comms
int i_ammo;                // Stores remaining ammo to be passed to jetson via serial comms
bool b_flag = false;       // Flag changes to true when jetson serial comms is established
bool b_trigger = false;    // Enables the firing action
bool b_movement = false;   // Enables the movement action
char c_out[30];            // Contains verbose X and Y co-ordinates received from jetson via serial comms

void setup() 
{
  //Comms Setup
  Serial.begin(74880);
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  Serial.println("Machine Vision Foam Dart Sentry");
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  Wire.begin();
  jSerial.begin(9600);
  i_toSend = 1; //Connection establishment: Sets "toSend" to 1 which means "Connection Established". This then sets the flag to true so that it does not set "toSend" to 1 again.
  b_flag = true;

  //Firing Motor Setup
  digitalWrite(ci_motorPin, 0);

  //Servo Pin Setup
  magServo.attach(ci_servoPin);
  magServo.write(cb_servoOut);

  //Stepper Setups
  magStepper.setMaxSpeed(ci_maxMagSpeed);

  //Output pins
  pinMode(ci_panStepPin, OUTPUT);
  pinMode(ci_panDirPin, OUTPUT);
  pinMode(ci_tiltStepPin, OUTPUT);
  pinMode(ci_tiltDirPin, OUTPUT);
  pinMode(ci_magStepPin, OUTPUT);
  pinMode(ci_magDirPin, OUTPUT);
  pinMode(ci_motorPin, OUTPUT);

  //Input pins
  pinMode(ci_panHomeHallPin, INPUT);
  pinMode(ci_tiltHomeHallPin, INPUT);
  pinMode(ci_tiltDownHallPin, INPUT);
  pinMode(ci_tiltUpHallPin, INPUT);



  //Dev
  panStepper.setMaxSpeed(1000.0);    //This sets the maximum speed, datatype is float.
  panStepper.setAcceleration(25.0);  //This sets the acceleration and deceleration rates in steps/s, datatype is float.
  tiltStepper.setMaxSpeed(1000.0);   //This sets the maximum speed, datatype is float.
  tiltStepper.setAcceleration(25.0); //This sets the acceleration and deceleration rates in steps/s, datatype is float.

  homing_func(); // Homes the mechanism before entering the main program loop
  home_mag(); // Homes the magazine lining up the first dart
}

//Loop Function
void loop() 
{
  scanning_func();
}


//---------------- Custom Functions ----------------\\

void home_mag() //Contains the function for homing the magazine to dart zero
{
  goToDart(0);
}

void goToDart(byte dart) //Contains the function for selecting a dart
{
  while(1)
  {
    delay(30);
    int i_tmp = magSensor.getRawAngle();
    int i_d = ci_dartPos[dart] - i_tmp;
    if ( i_tmp > (ci_dartPos[dart] - 3) && i_tmp < (ci_dartPos[dart] + 3)) 
    {
      return 0;
    }
    else if ( i_tmp > ci_dartPos[dart]) 
    {
      magStepper.setSpeed(50); 
      magStepper.runSpeed();
    }
    else 
    {
      magStepper.setSpeed(-50); 
      magStepper.runSpeed();
    }
  }
}

void spinUp() //Contains the function for spinning up the firing motors
{
  motorRamp.go(0);
  motorRamp.go(cb_vMaxPWM, ci_motorSpinUpPeriod);
  while(motorRamp.update() != cb_vMaxPWM)
  {
    analogWrite(ci_motorPin, motorRamp.update());
  }
}

void spinDown() //Contains the function for spinning down the firing motors
{
  analogWrite(ci_motorPin, 0);
}

void fire() //Contains the function to fire the selected dart
{
  magServo.write(cb_servoIn);
  delay(400);
  magServo.write(cb_servoOut);
  delay(400);
}

void homing_func() //Contains the function for homing the pan/tilt head
{
  /*
    -- Homing Function --
    The code in this funciton will drive the pan/tilt head to its 'home' hall effect sensors.
    
    - Homing the tilt axis -
    (1) The tilt axis will first be poiting down as this is more secure for transit
    (2) Drive tilt axis up at a constant speed until the 'tiltUpHallPin' is detected 'low', then record current coordinate
    (3) Drive tilt axis down at constant speed until the 'tiltDownHallPin' is detected 'low', then record current coordinates
    (4) Calculate the midpoint between the coordinates for (2) and (3)
    (5) Drive tilt axis to calculated midpoint and stop tilt axis

    - Homing the pan axis -
    (1) Drive the pan axis around at a constant speed until 'panHomehallPin' is detected 'low' 
    (2) When 'panHomeHallPin' is detected 'low', stop the pan axis and record current coordinate

  */

  //Tilt Up till Limit 
  tiltStepper.setSpeed(-100);
  while(digitalRead(ci_tiltUpHallPin) == 1) // Hall effect output is low when magnet is detected
  {
    tiltStepper.runSpeed();
  }
  tiltStepper.stop();
  i_tiltUpHallDetect = tiltStepper.currentPosition();
  delay(200);

  //Tilt Down till Limit
  tiltStepper.setSpeed(100);
  while(digitalRead(ci_tiltDownHallPin) == 1)
  {
    tiltStepper.runSpeed();
  }
  tiltStepper.stop();
  i_tiltDownHallDetect = tiltStepper.currentPosition(); 
  delay(200);

  //Calculate mid then move to.
  if (i_tiltDownHallDetect > i_tiltUpHallDetect) 
  {
    i_tiltHomeHallDetect = ((i_tiltDownHallDetect - i_tiltUpHallDetect) / 2) + i_tiltUpHallDetect;
  }
  else 
  {
    i_tiltHomeHallDetect = ((i_tiltUpHallDetect - i_tiltDownHallDetect) / 2) + i_tiltDownHallDetect;
  }
  tiltStepper.moveTo(i_tiltHomeHallDetect);
  while(tiltStepper.currentPosition() != i_tiltHomeHallDetect)
  {
    tiltStepper.run();
  }
  tiltStepper.stop();
  // Possibly add 'i_tiltHomeHallDetect = tiltStepper.currentPosition(0);' here?
  // Parsing '0' into 'currentPosition' will set the current location as the Zero point for further coordinates which could be useful as.
  // Though this is possibly more relevant for simplifying 'tracking_func' logic, current implementation does work.
  delay(200);

  //Home Pan
  panStepper.setSpeed(100);
  while(digitalRead(ci_panHomeHallPin) == 1)
  {
    panStepper.runSpeed();
  }
  panStepper.stop();
  i_panHomeHallDetect = panStepper.currentPosition(); // If we parse '0' into currentPosition, the current location will be set as coordinate 0
  delay(200);
  
}// end of homing_func

void scanning_func() //Contains the function for 'scanning' for potential tartets
{
  /*
    -- Scanning Function --
    The code in this function drives pan and tilt axes between coordinates either side of their home positions.
    This will allow for the machine vision system to get a better view of potential targets infront of the system.

  */

  //Pan Stepper Control
  if(b_panScanDir == 0)
  {
    panStepper.moveTo(i_panHomeHallDetect - i_panScanRange);
  }
  else
  {
    panStepper.moveTo(i_panHomeHallDetect + i_panScanRange);
  }

  if(panStepper.distanceToGo() == 0) // 'distanceToGo' returns how many steps are left until the target value is met
  {
    b_panScanDir = !b_panScanDir;    // Setting this will cause axis to rotate in opposite direction
  }
    
  //Tilt Stepper Control
  if(b_tiltScanDir == 0)
  {
    tiltStepper.moveTo(i_tiltHomeHallDetect - i_tiltScanRange);
  }
  else
  {
    tiltStepper.moveTo(i_tiltHomeHallDetect + i_tiltScanRange);
  }

  if(tiltStepper.distanceToGo() == 0)
  {
    b_tiltScanDir = !b_tiltScanDir;
  }

  panStepper.run();
  tiltStepper.run();
}

void tracking_func() //Contains the function for 'tracking' a target once it has been identified
{
  if(Serial.available() > 0)
  {
    /* 
      Add code here that accepts users coordinates
      Checks to see if inputted coordinates are within bounds of acceptable movement
      And drives mechanism to user specified coordinates
    */
    if(i_tiltTargetCoord > -133 || i_tiltTargetCoord > 133)
    {
        Serial.println("Please enter a value between -133 (Look up) and 133 (Look down).");
    }
  }
} //End of tracking_func

// Function that sends data to Master (Pi). Unlike receiveEvent, the Wire.available confirmation loop cannot be used as it stops data being sent.
void requestEvent() {
  jSerial.println(i_toSend);
}

// Function that deals with status codes to control Arduino
void statusCodes() {
  //Static keyword used to create variable for only one function. This, unlike other variables, keeps its value after function calls, allowing it to
  // be compared with newer versions of values to make comparisons with them. This is used in this code to avoid the servo moving over to the same position again
  // even though it is receiving the same coordinates.
  static int x_old = 0;
  static int y_old = 0;

  if (s_received == "1") {
    Serial.println("Raspberry Pi - Connection Established");
  } else if (s_received == "2") {
    Serial.println("Raspberry Pi - Data Received");
  } else if (s_received == "100") {
    Serial.println("Raspberry Pi - Status OK");
  } else if (s_received[0] == '(' ) {
    Serial.println("Coordinates Received, Ready to Delete Target!");
    //sscanf is used here to read formatted input from a string, then assigns the values to be read onto integer varianbles.
    // in this case, they are set to coordinate "x" and "y" variables.
    int r = sscanf(s_received.c_str(), "(%d, %d)", &i_x, &i_y);
    snprintf(c_out, 30, "X: %d, Y: %d", i_x, i_y);

    Serial.print("X: ");
    Serial.println(i_x);
    Serial.print("Y: ");
    Serial.println(i_y);

    // Below conditional statement checks if the current coordinate variables are equal to its previous versions stored on the static int variables.
    // In the case it is, the movement flag is set to false, blocking movement from occuring as no change has occured.
    // However, in the case that it is not the same as its previous version, movement is set to true, then the coordinate variables are stored in the static int variables,
    // so as to be compared to the next values sent.
    if (i_x == x_old && i_y == y_old) {
      b_movement = false;
    } else {
      b_movement = true;
      x_old = i_x;
      y_old = i_y;
    }
  }
}

//The Below function works on blocking strings/characters from appearing except brackets and numbers.
void convertToInt() {
  int n = s_received.toInt();
  if (n > 0 || s_received[0] == '(') {
    //Serial.println(received_str);
    s_received = "";
  } else {
    s_received = "";
  }
}

/* The below function handles target detection.
   It first maps the values from 0 to 1023(current limit) to 0 to 180. This is done due the servo being limited at ~180 degrees.
   While it is true that it could instead be done from -90 to 90, doing this does not work, as it limits the servo for some reason into just 90 degrees.
   The conditional statement within the function first checks if movement is true. As you remember, movement was the flag within the
   statusCodes function to check if the values being received were the same or were different. If they were different, it would set "movement" to true.
   Thus, the below statement checks if this flag is true, then writes on to both servo "x" and "y" the values that were sent over I2C.
   After it has finished writing the values, it sets movement to false to block any further movements.
*/
void onDetectTarget() {
  i_x = map(i_x, 0, 1023, 0, 180);
  i_y = map(i_y, 0, 1023, 0, 180);
  if (b_movement == true) {
    //myservo.write(i_x);
    //myservo2.write(i_y);
    /* Responsible for handling shooting */
      if (i_ammo > 3) {
      b_trigger = true;
      //start shooting
      } else {
      b_trigger = false;
      }
    delay(15);
    b_movement = false;
  }
}
