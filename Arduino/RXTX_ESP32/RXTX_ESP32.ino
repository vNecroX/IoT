#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN_GPIO5 5     
#define DHTTYPE DHT11 
DHT dht(DHTPIN_GPIO5, DHTTYPE);
float h, t;
String percent = String("%");
String celsius = String("°C");

int PHOTO_GPIO4 = 4;
int lumens;
String luminosityLevel = String(" lumenes");

#define SOUND_SPEED 0.034
int ULTRAtrig_GPIO18 = 18;
int ULTRAecho_GPIO19 = 19;
long pulseDuration;
float distance;
String cm = String("cm");

void setup(){
  Serial.begin(9600);
  
  dht.begin();

  pinMode(PHOTO_GPIO4, INPUT);

  pinMode(ULTRAtrig_GPIO18, OUTPUT);
  pinMode(ULTRAecho_GPIO19, INPUT);
}

void loop(){
  Serial.println("H & T / / / / / / / / / / / / / / / /");
  readHT();

  Serial.println("PHOTO / / / / / / / / / / / / / / / /");
  readLuminosity();
  
  Serial.println("ULTRA / / / / / / / / / / / / / / / /");
  readDistance();

  Serial.println("JSON  / / / / / / / / / / / / / / / /");
  createJSON();

  delay(2000);
}

void readHT(){
  h = dht.readHumidity();
  t = dht.readTemperature();  

  if(isnan(h) || isnan(t)){
    Serial.println("Error retrieving data form sensor DHT11");
  }
  else{
    Serial.print("Humidity %: ");
    Serial.println(h);
    
    Serial.print("Temperature C: ");
    Serial.print(t);
  }

  Serial.print("\n\n");
}

void readLuminosity(){
  lumens = analogRead(PHOTO_GPIO4);
  Serial.print("Luminosity level: ");
  Serial.println(lumens);
  Serial.print("\n");
}

void readDistance(){
  // Clears the ULTRAtrig_GPIO18
  digitalWrite(ULTRAtrig_GPIO18, LOW);
  delayMicroseconds(10);
  
  // Sets the ULTRAtrig_GPIO18 on HIGH state for 10 micro seconds
  digitalWrite(ULTRAtrig_GPIO18, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRAtrig_GPIO18, LOW);

  // Reads the ULTRAecho_GPIO19, returns the sound wave travel time in microseconds
  pulseDuration = pulseIn(ULTRAecho_GPIO19, HIGH, 4000000);
  Serial.print("Pulse duration: ");
  Serial.println(pulseDuration);

  // The speed of sound is 340m/s which is equal to 34cm/ms = 34/1000 = 0.034
  distance = (pulseDuration * SOUND_SPEED) / 2;
  Serial.print("Distance cm: ");
  Serial.println(distance);
  Serial.print("\n");
}

void createJSON(){
  StaticJsonDocument<300> JSON_Hencoder;
  String hString = String();
  hString = h + percent;
  
  JSON_Hencoder["tipoDeDato"] = "Humedad";
  JSON_Hencoder["resultado"] = hString;

  StaticJsonDocument<300> JSON_Tencoder;
  String tString = String();
  tString = t + celsius;
  
  JSON_Tencoder["tipoDeDato"] = "Temperatura";
  JSON_Tencoder["resultado"] = tString;

  StaticJsonDocument<300> JSON_HTencoder;
  JSON_HTencoder["nombre"] = "Sensor de Humedad";
  JsonArray dataHT = JSON_HTencoder.createNestedArray("datos");
  dataHT.add(JSON_Hencoder);
  dataHT.add(JSON_Tencoder);

  StaticJsonDocument<300> JSON_PHOTOencoder0;
  String photoString = String();
  photoString = lumens + luminosityLevel;

  JSON_PHOTOencoder0["tipoDeDato"] = "Luminosidad";
  JSON_PHOTOencoder0["resultado"] = photoString;

  StaticJsonDocument<300> JSON_PHOTOencoder1;
  JSON_PHOTOencoder1["nombre"] = "Sensor de luz";
  JsonArray dataPHOTO = JSON_PHOTOencoder1.createNestedArray("datos");
  dataPHOTO.add(JSON_PHOTOencoder0);

  StaticJsonDocument<300> JSON_ULTRAencoder0;
  String ultraString = String();
  ultraString = distance + cm;

  JSON_ULTRAencoder0["tipoDeDato"] = "Distancia";
  JSON_ULTRAencoder0["resultado"] = ultraString;

  StaticJsonDocument<300> JSON_ULTRAencoder1;
  JSON_ULTRAencoder1["nombre"] = "Sensor de ultrasonido";
  JsonArray dataULTRA = JSON_ULTRAencoder1.createNestedArray("datos");
  dataULTRA.add(JSON_ULTRAencoder0);

  StaticJsonDocument<400> JSON_Encoder;
  JSON_Encoder.add(JSON_HTencoder);
  JSON_Encoder.add(JSON_PHOTOencoder1);
  JSON_Encoder.add(JSON_ULTRAencoder1); 
  serializeJsonPretty(JSON_Encoder, Serial);

  Serial.print("\n\n");
}
