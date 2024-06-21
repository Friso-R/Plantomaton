#include "Broker.h"
#include "Sensors.h"
#include "Fans.h"

Sensors sensors;
Fans fans;

void setup() {
  sensors.setup();
  fans.setup();
}

void loop() {
  sensors.update();
  fans.loop();
}

// This function is executed when some device publishes a message to a topic that the ESP32 is subscribed to
void callback(String topic, byte* message, unsigned int length) {
  String messageTemp;
  
  for (int i = 0; i < length; i++)  
    messageTemp += (char)message[i];

  if(topic == "infob3it/student033/gordijn"){
    //if(messageTemp == "start")
      //stepMotor.start();
  }
}