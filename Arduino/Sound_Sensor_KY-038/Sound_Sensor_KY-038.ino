const int LED = 13;
const int soundRead = A1;

int soundLevel;
const int threshold = 195;

void setup(){
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  Serial.println("/ / / / / / / / / / / / / / / /");
  
  soundLevel = analogRead(soundRead);
  Serial.print("Sound level: ");
  Serial.println(soundLevel);
  Serial.print("\n");

  if(soundLevel >= threshold) digitalWrite(LED, HIGH);
  else digitalWrite(LED, LOW);

  delay(500);
}
