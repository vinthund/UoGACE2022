//int Setpoint_X, Setpoint_Y;
#define ENA 2
#define IN1 3
#define IN2 4
String command;

//#define BL 2
void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
  Serial.begin(9600);
  //Serial.print("Hello, This is program to get X Y setpoint value!\r\n");
}

void loop() {
  // read the value from the sensor:
  //Serial.print("Setpoint_X: ");
  //Serial.print(analogRead(A1));
  //Serial.print("      ");
  //Serial.print("Setpoint_Y: ");
  Serial.println(analogRead(A0));
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();

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

    else if (command.equals("off")) {
      digitalWrite(ENA, LOW);
      digitalWrite(IN1, LOW);
      digitalWrite(IN1, HIGH);
    }
  }
  //Serial.print("\r\n");
  delay(100);
}
