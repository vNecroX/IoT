 #include <WiFi.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// Own network credentials
const char* ssid = "INFINITUM0F03_2.4";
const char* password = "5327435555";

#define DHTPIN_GPIO5 5     
#define DHTTYPE DHT11 
DHT dht(DHTPIN_GPIO5, DHTTYPE);
float h, t;
String percent = String("%");
String celsius = String("°C");

int PHOTO_GPIO34 = 34;
int lumens;
String luminosityLevel = String(" lumenes");

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

  pinMode(PHOTO_GPIO34, INPUT);

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

  Serial.print("jsonDataEncoder: ");
  Serial.println(jsonDataEncoder);

  sendJSON(requestBody);
  
  Serial.print("\n");
}

void sendJSON(String request){
  HTTPClient http;
  
  http.begin(serverPath);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  int httpResponseCode = http.POST(request);
  String response = http.getString();
  
  if(httpResponseCode > 0){
    Serial.println(response);
  }else{
    Serial.println("Error on HTTP request");
    Serial.print("\n");
    Serial.println(response);
  }

  http.end();
}
