#ifndef SETUP_WIFI_H
#define SETUP_WIFI_H

#include <WiFi.h>
#include "time.h"

class WiFiSetup {
private:
  //const char* ssid = "Appels";
  //const char* password = "R!kwjH0acuEP8JE";
const char* ssid = "A-je-to! 2.4";
const char* password = "HoldTheDoor!187";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

// Static function for handling the WiFi disconnection event
static void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  //Serial.println("Disconnected from WiFi access point");
  WiFi.reconnect();
}

public:
  // Constructor to set up the WiFi event handler
  WiFiSetup() {
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    connect();

    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  }

  // This function connects ESP32 to router
  void connect() {
    int tries = 3;
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      //Serial.println(".");
      delay(3000);
      tries--;
      if (tries == 0) ESP.restart();
    }
    Serial.println("WiFi connected");
  }

  int nowTimeMin()
  {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return 0;
    }
    //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S"); 
    return timeinfo.tm_hour * 60 + timeinfo.tm_min;
  }
};

#endif
