const int trigPin = 3;
const int echoPin = 2;

long pulseDuration;
int distance;

void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  Serial.println("/ / / / / / / / / / / / / / / /");

  // Reads the echoPin, returns the sound wave travel time in microseconds
  // The speed of sound is 340m/s which is equal to 34cm/ms = 34/1000 = 0.034
  pulseDuration = pulseIn(echoPin, HIGH);
  Serial.print("Pulse duration: ");
  Serial.println(pulseDuration);
  
  distance = pulseDuration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("\n");

  delay(1000);
}
