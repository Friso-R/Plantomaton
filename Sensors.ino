#include "Broker.h"
#include "Sensors.h"

Sensors sensors;

void setup() {
  sensors.setup();
}

void loop() {
  sensors.update();
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