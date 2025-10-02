#ifndef DHTx_H
#define DHTx_H

#include "DHT.h"

#define DHTPIN 33 
#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

class DHT21xxx{
public:
  float t;
  float h;

  void setup() {
    Serial.println("DHT21 test!");
    dht.begin();
  }

  void loop() {
    h = dht.readHumidity();
    t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(F("Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.println("");
  }
};

#endif