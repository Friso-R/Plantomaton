#ifndef broker_h
#define broker_h

#include <WiFi.h>
#include <PubSubClient.h>

// wifi credentials
//const char* ssid = "A-je-to! 2.4";
//const char* password = "HoldTheDoor!187";
const char* ssid = "Appels_IoT";
const char* password = "GoedWachtwoord51";

// MQTT broker credentials
const char* MQTT_username = "leendertr"; 
const char* MQTT_password = "Halt2001"; 
const char* mqtt_server = "192.168.1.213";

//const char* MQTT_username = "student033"; 
//const char* MQTT_password = "U5rlK4N8"; 
//const char* mqtt_server = "science-vs352.science.uu.nl";

// Initializes the espClient
WiFiClient    espClient;
PubSubClient  client(espClient);
//BlockNot      PublishData(5,  SECONDS);

extern void callback(String topic, byte* message, unsigned int length);

class Broker
{

public: 
  void begin(){
    client.setCallback(callback); 
    client.setServer(mqtt_server, 1883);
    client.subscribe("infob3it/student033/pomp");
    client.subscribe("infob3it/student033/lamp");
    client.subscribe("infob3it/student033/humi");
    client.subscribe("infob3it/student033/optimal");

  }

  void update(){ client.loop(); }

  void publish(String topic, String message) {

    if (!client.connected()) 
      reconnect();
      
    if(!client.loop())
      client.connect("ESP32Greenhouse", MQTT_username, MQTT_password);

    topic = "infob3it/student033/" + topic;
    client.publish(topic.c_str(),   String(message).c_str());
  } 

private:
  // This functions reconnects your ESP32 to your MQTT broker
  // Change the function below if you want to subscribe to more topics with your ESP8266 
  void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("ESP32Greenhouse", MQTT_username, MQTT_password)) {
        Serial.println("connected");  
        // Subscribe or resubscribe to a topic
        // You can subscribe to more topics
        client.subscribe("infob3it/student033/pomp");
        client.subscribe("infob3it/student033/lamp");
        client.subscribe("infob3it/student033/humi");
        client.subscribe("infob3it/student033/optimal");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }
};

#endif