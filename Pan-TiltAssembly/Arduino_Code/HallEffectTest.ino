int hall_left = 2;
int hall_mid = 3;
int hall_rght = 4;

int hall_left_detect = 0;
int hall_mid_detect = 0;
int hall_rght_detect = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(hall_left, INPUT);
  pinMode(hall_mid, INPUT);
  pinMode(hall_rght, INPUT);
}

void loop() 
{
  hall_left_detect = digitalRead(hall_left);
  hall_mid_detect = digitalRead(hall_mid);
  hall_rght_detect = digitalRead(hall_rght);

  if(hall_left_detect == LOW)
  {
    Serial.println("Magnet detected - left!");
  }

  if(hall_mid_detect == LOW)
  {
    Serial.println("Magnet detected - middle!");
  }

  if(hall_rght_detect == LOW)
  {
    Serial.println("Magnet detected - right!");
  }
  
}