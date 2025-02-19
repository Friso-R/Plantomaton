#ifndef broker_h
#define broker_h

#include <PubSubClient.h>

WiFiClient    espClient;
PubSubClient  client(espClient);

extern void callback(String topic, byte* message, unsigned int length);

class Broker
{
private:
  const char* MQTT_username = "leendertr"; 
  const char* MQTT_password = "Halt2001"; 
  const char* MQTT_server   = "server-cam.duckdns.org";
   //const char* MQTT_server   = "192.168.1.220";

  void subscriptions(){
    client.subscribe("kas/#");
  }

  void connect() {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");

      client.connect("ESP32KasClient", MQTT_username, MQTT_password);
      delay(2000);
      subscriptions();
    }
    Serial.println("connected"); 
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
      WiFi.reconnect();
      connect();
    }
      
    
    topic = "kas/" + topic;
    client.publish(topic.c_str(),   message.c_str());
  }
};

#endif