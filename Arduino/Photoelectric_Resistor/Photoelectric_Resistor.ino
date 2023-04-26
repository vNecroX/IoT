const int lumRead = A1;

int lumLevel;

void setup(){
  Serial.begin(9600);
  pinMode(lumRead, INPUT);
}

void loop(){
  Serial.println("/ / / / / / / / / / / / / / / /");

  lumLevel = analogRead(lumRead);
  Serial.print("Luminosity level: ");
  Serial.println(lumLevel);
  Serial.print("\n");

  delay(1000);
}
