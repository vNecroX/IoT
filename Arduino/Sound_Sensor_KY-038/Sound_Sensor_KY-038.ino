const int LED = 13;
const int soundRead = A1;

float randomN;

float soundLevel;
const float threshold = 64.5;

void setup(){
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  Serial.println("/ / / / / / / / / / / / / / / /");

  randomN = random(1, 60);
  randomN /= 100;
  Serial.print("Random: ");
  Serial.println(randomN);
    
  soundLevel = analogRead(soundRead);
  soundLevel += randomN;
  Serial.print("Sound level: ");
  Serial.println(soundLevel);
  Serial.print("\n");

  // Our limit is 64.5, if sound level is equal or exceed it, then, based on the random number
  // there is a reduced chance to get and 50 or higher number up to 60, in other words
  // 1/6 or 0.16 chances to turn on the LED as a detected sound
  if(soundLevel >= threshold) digitalWrite(LED, HIGH);
  else digitalWrite(LED, LOW);

  delay(500);
}
