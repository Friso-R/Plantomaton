#ifndef broker_h
#define broker_h

#include <WiFi.h>
#include <PubSubClient.h>
//#include <BlockNot.h>

// wifi credentials
//const char* ssid = "A-je-to! 2.4";
//const char* password = "HoldTheDoor!187";
const char* ssid = "Appels";
const char* password = "R!kwjH0acuEP8JE";

// MQTT broker credentials
const char* MQTT_username = "student033"; 
const char* MQTT_password = "U5rlK4N8"; 

const char* mqtt_server = "science-vs352.science.uu.nl";

// Initializes the espClient
WiFiClient    espClient;
PubSubClient  client(espClient);
//BlockNot      PublishData(5,  SECONDS);

extern void callback(String topic, byte* message, unsigned int length);

class Broker
{

public: 
  bool Auto;
  
  void begin(){
    Auto = 0;
    setup_wifi();
    client.setCallback(callback); 
    client.setServer(mqtt_server, 1883);
    client.subscribe("infob3it/student033/pomp");
    client.subscribe("infob3it/student033/lamp");
    client.subscribe("infob3it/student033/humi");
    client.subscribe("infob3it/student033/optimal");

  }

  // This functions connects ESP32 to router
  void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected - ESP IP address: ");
    Serial.println(WiFi.localIP());
  }

  void update(){
    //if (PublishData.TRIGGERED) 
      //publish();
    client.loop();
  }

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