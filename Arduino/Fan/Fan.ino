int FAN_GPIO25 = 25;
int fanState = 0;

void setup(){
  Serial.begin(9600);

  pinMode(FAN_GPIO25, OUTPUT);
}

void loop(){
  if(fanState == 0){
    fanState = 1;
    digitalWrite(FAN_GPIO25, HIGH);
  }
  else{
    fanState = 0;
    digitalWrite(FAN_GPIO25, LOW);
  }
  Serial.print("Fan state: ");
  Serial.println(fanState);
  Serial.print("\n");

  delay(2000);
}
