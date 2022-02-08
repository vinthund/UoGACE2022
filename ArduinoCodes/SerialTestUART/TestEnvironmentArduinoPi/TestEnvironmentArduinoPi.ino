/*This code was taken and modified from my (Joel) Individual project. This code basically manipulates an H bridge whenever receiving commands from Python, flipping polarity depending on the type
 * of command given e.g. "positive", "negative" and "off".
 * 
 * Version: 1.3
*/
String command;
#define ENA 2
#define IN1 3
#define IN2 4
int counter = 1;

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  /* Below Code constantly prints the data picked from A0 to Serial. This will then be processed by Python to
   *  manipulate through conditional statements and send back to generate an output on the Arduino. The sensor
   *  that is connected to pin A0 is a hall effect sensor, which is wired up with an op-amp, voltage divider and a potentiometer (for personal project reasons)
   */
  Serial.println(analogRead(A0));
  if (Serial.available()) {
    // Checks if Serial is available before scanning for signals

    //The string "command" reads the commands and ignores blank spacing through the use of ".readStringUntil"
    command = Serial.readStringUntil('\n');
    command.trim();

    if (counter == 1) {
      Serial.println("Requesting Connection");
      counter -=1;
    }
    
    // Conditional statements that check the outputs given from "command"; if it is positive, then set IN1 HIGH, if the response is negative then set IN2 HIGH instead.
    if (command.equals("positive")) {
      digitalWrite(ENA, HIGH);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } 

    else if (command.equals("negative")) {
      digitalWrite(ENA, HIGH);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    } 
    //Else, set the pins for both IN1 and IN2 low when a certain condition is achieved on Python.
    else if (command.equals("off")) {
      digitalWrite(ENA, LOW);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }
  }
  //Delays by 0.1 seconds
  delay(800);
  if (counter == 0) {
    Serial.println("Arduino Connected");
  }
}
