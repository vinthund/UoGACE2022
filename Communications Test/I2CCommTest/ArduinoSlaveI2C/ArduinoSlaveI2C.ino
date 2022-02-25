/*This code was taken and modified from my (Joel) Individual project. This code basically manipulates an H bridge whenever receiving commands from Python, flipping polarity depending on the type
   of command given e.g. "positive", "negative" and "off". This is the I2C Version of the UART code made earlier.
 * SDA <--> SDA
 * SCL <--> SCL
 * GND <--> GND
 * Version: 1.1
*/

// Include the Wire library for I2C
#include <Wire.h>

#define ENA 2
#define IN1 3
#define IN2 4
#define ledPin 13
bool count = false;


void setup() {
  // Join I2C bus as slave with address 8
  Wire.begin(0x8);

  // Call receiveEvent when data received
  Wire.onReceive(receiveEvent);
  //pinMode definitons for H bridge to be possibly used for later.
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  Serial.begin(9600);
}

// Function that executes whenever data is received from master
void receiveEvent(int howMany) {
  while (Wire.available()) { // loop through all but the last

    char c = Wire.read(); // receive byte as a character
    Serial.print(c);
    digitalWrite(ledPin, c);

  }
}

void loop() {
  delay(100);
  //Serial.print("Hello World");
}

/*
 * Below are the codes that are being used as inspiration to improve bi-directional communication between the Arduino and the Raspberry Pi. They were found online from others who also worked 
 * on I2C communication.
 * Source for Code 1: https://www.aranacorp.com/en/communication-between-raspberry-pi-and-arduino-with-i2c/
 * Source for Code 2: https://roboticsbackend.com/raspberry-pi-master-arduino-slave-i2c-communication-with-wiringpi/
 */


/*
 * Code 1:
  #include <Wire.h>
  #define I2C_SLAVE_ADDRESS 0x08
  #define PAYLOAD_SIZE 2
  void setup()
  {
  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(9600);
  Serial.println("-------------------------------------I am Slave1");
  delay(1000);
  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);
  }


  void loop() {}
  int n = 0;
  void requestEvents()
  {
  Serial.println(F("---> recieved request"));
  Serial.print(F("sending value : "));
  Serial.println(n);
  Wire.write(n);
  }
  void receiveEvents(int numBytes)
  {
  Serial.println(F("---> recieved events"));
  n = Wire.read();
  Serial.print(numBytes);
  Serial.println(F("bytes recieved"));
  Serial.print(F("recieved value : "));
  Serial.println(n);
  }


*/


/* Code 2:
 * #include <Wire.h>

  #define SLAVE_ADDRESS 0x08

  byte data_to_echo = 0;

  void setup()
  {
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  }

  void loop() { }

  void receiveData(int bytecount)
  {
  for (int i = 0; i < bytecount; i++) {
    data_to_echo = Wire.read();
  }
  }

  void sendData()
  {
  Wire.write(data_to_echo);
  }
*/
