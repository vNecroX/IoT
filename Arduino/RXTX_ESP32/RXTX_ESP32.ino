#include <WiFi.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// Own network credentials
const char* ssid = "INFINITUM1D2D_2.4";
const char* password = "6111611161116";

#define DHTPIN_GPIO5 5     
#define DHTTYPE DHT11
DHT dht(DHTPIN_GPIO5, DHTTYPE);
float h, t;
String percent = String("%");
String celsius = String("°C");

int FAN_GPIO25 = 25;

int PHOTO_GPIO34 = 34;
int lumens;
String luminosityLevel = String(" lumenes");

int LEDmainDoor_GPIO21 = 21;
int LEDroom1_GPIO4 = 4;
int LEDroom2_GPIO15 = 15;

#define SOUND_SPEED 0.034
int ULTRAtrig_GPIO18 = 18;
int ULTRAecho_GPIO19 = 19;
long pulseDuration;
float distance;
String cm = String("cm");

String serverPath = "https://ceti-iiot-codemasterx.000webhostapp.com/update_data.php";

void setup(){
  Serial.begin(9600);

  connectToWifi();
  
  dht.begin();

  pinMode(FAN_GPIO25, OUTPUT);

  pinMode(PHOTO_GPIO34, INPUT);

  pinMode(LEDmainDoor_GPIO21, OUTPUT);
  pinMode(LEDroom1_GPIO4, OUTPUT);
  pinMode(LEDroom2_GPIO15, OUTPUT);

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

  delay(5000);
}

void connectToWifi(){
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
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

  Serial.print("\n");
}

void readLuminosity(){
  lumens = analogRead(PHOTO_GPIO34);
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
  
  // Two ways to do the same
  String jsonDataEncoder = JSON_Encoder.as<String>();
  // String jsonDataEncoder;
  // serializeJson(JSON_Encoder, jsonDataEncoder);
  String requestBody = "data=" + jsonDataEncoder;

  Serial.print("jsonData: ");
  Serial.println(jsonDataEncoder);

  sendJSON_zero(requestBody);
  
  Serial.print("\n");
}

void sendJSON_zero(String request){
  HTTPClient http; 

  http.begin(serverPath);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  

  int httpResponseCode = http.POST(request);
  String response = http.getString();
  
  if(httpResponseCode > 0){
    Serial.println(response);
    decEncResponseJSON(response);
  }else{
    Serial.println("Error on HTTP request");
    Serial.print("\n");
    Serial.println(response);
  }

  http.end();
}

void decEncResponseJSON(String response){
  StaticJsonDocument<300> JSON_Decoder;
  DeserializationError error = deserializeJson(JSON_Decoder, response);
  if(error){ return; }

  float idealTemperature = JSON_Decoder["temperaturaIdeal"];
  String mainDoor = JSON_Decoder["puertaPrincipal"];
  String room1 = JSON_Decoder["salon1"];
  String room2 = JSON_Decoder["salon2"];
  int lumensToON = JSON_Decoder["luminosidadEncender"];
  int lumensToOff = JSON_Decoder["luminosidadApagar"];
  String fan = JSON_Decoder["ventilador"];

  if(t >= idealTemperature) fan = "on";
  else fan = "off";

  
  if(fan.equals("on")) pinMode(FAN_GPIO25, OUTPUT);
  if(fan.equals("off")) pinMode(FAN_GPIO25, INPUT);

  if(lumens <= lumensToON){
    mainDoor = "on";
    room1 = "on";
    room2 = "on";
  }
  else if(lumens >= lumensToOff){
    mainDoor = "off";
    room1 = "off";
    room2 = "off";
  }

  if(mainDoor.equals("on")) digitalWrite(LEDmainDoor_GPIO21, HIGH);
  if(room1.equals("on")) digitalWrite(LEDroom1_GPIO4, HIGH);
  if(room2.equals("on")) digitalWrite(LEDroom2_GPIO15, HIGH);

  if(mainDoor.equals("off")) digitalWrite(LEDmainDoor_GPIO21, LOW);
  if(room1.equals("off")) digitalWrite(LEDroom1_GPIO4, LOW);
  if(room2.equals("off")) digitalWrite(LEDroom2_GPIO15, LOW);

  String json;
  StaticJsonDocument<300> JSON_Encoder;

  JSON_Encoder["temperaturaIdeal"] = idealTemperature;
  JSON_Encoder["puertaPrincipal"] = mainDoor;
  JSON_Encoder["salon1"] = room1;
  JSON_Encoder["salon2"] = room2; 
  JSON_Encoder["luminosidadEncender"] = lumensToON;
  JSON_Encoder["luminosidadApagar"] = lumensToOff; 
  JSON_Encoder["ventilador"] = fan;
  
  String jsonDataEncoder = JSON_Encoder.as<String>();
  String requestBody = "configuracion=" + jsonDataEncoder;

  Serial.print("jsonData: ");
  Serial.println(jsonDataEncoder);

  sendJSON_one(requestBody);
  
  Serial.print("\n");
}

void sendJSON_one(String request){
  HTTPClient http; 

  http.begin(serverPath);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  

  int httpResponseCode = http.POST(request);
  String response = http.getString();

  if(httpResponseCode > 0){
    Serial.println("Updated HTTP request");
    Serial.print("\n");
  }else{
    Serial.println("Error on HTTP request");
    Serial.print("\n");
    Serial.println(response);
  }

  http.end();
}
