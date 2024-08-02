#ifndef SETUP_WIFI_H
#define SETUP_WIFI_H

#include <WiFi.h>

class WiFiSetup{
private:
  //const char* ssid = "Appels";
  //const char* password = "R!kwjH0acuEP8JE";
  const char* ssid = "A-je-to! 2.4";
  const char* password = "HoldTheDoor!187";

public:
   // This functions connects ESP32 to router
  void connect() {
    int tries = 3;
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      Serial.println(".");
      uint8_t status = WiFi.waitForConnectResult();
      tries--;
      if(tries == 0) ESP.restart();
    }
    Serial.println("WiFi connected");
  }
};

#endif