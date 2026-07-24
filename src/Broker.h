#pragma once

#include <PubSubClient.h>

WiFiClient    espClient;
PubSubClient  client(espClient);

extern void callback(String topic, byte* message, unsigned int length);

class Broker
{
private:
  const char* MQTT_username = "Kasclient"; 
  const char* MQTT_password = "Halt2001"; 
  const char* MQTT_server   = "192.168.1.150";

  void subscriptions(){
    client.subscribe("kas/#");
  }

  void connect() {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP32KasClient", MQTT_username, MQTT_password)) {
        Serial.println("connected");
        subscriptions(); 
    } else {
        Serial.print("failed, rc=");
        Serial.print(client.state()); // Print de reden waarom het mislukte
        Serial.println(" - trying again in 5 seconds");
        delay(5000);
      }
    }
    
  }

public: 
  void begin(){
    client.setCallback(callback); 
    client.setServer(MQTT_server, 1883);
    connect();
  }

  void update(){ client.loop(); }

  void publish(String topic, String message) {

    if (!client.connected()) {
     if (WiFi.status() != WL_CONNECTED) {
      WiFi.reconnect();
    }

  connect();
}
      
    
    topic = "kas/" + topic;
    client.publish(topic.c_str(),   message.c_str());
  }
};