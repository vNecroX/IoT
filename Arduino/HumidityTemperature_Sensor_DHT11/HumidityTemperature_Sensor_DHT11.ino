#include <DHT.h>

#define DHTPIN 2     
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

float h, t, f;

void setup(){
  Serial.begin(9600);
  dht.begin();
}

void loop(){
  Serial.println("/ / / / / / / / / / / / / / / /");
  
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true); // Temperature on F

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error retrieving data form sensor DHT11");
    return;
  }
 
  Serial.print("Humidity: ");
  Serial.println(h);
  
  Serial.print("Temperature C: ");
  Serial.print(t);
  Serial.print(", F: ");
  Serial.println(f);
  Serial.print("\n");

  delay(2000);
}
