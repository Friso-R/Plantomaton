#include "Broker.h"
#include "Sensors.h"
#include "Fans.h"
#include "Relay.h"

Sensors sensors;
Fans fans;

/* Define the relay pins
const int relay1 = 33;
const int relay2 = 25;
const int relay3 = 26;
const int relay4 = 27;
const int relay5 = 14;
const int relay6 = 12;
*/
Relay lamp(5);
Relay lampfans(14);
Relay humidifier(26);
Relay pomp(27);

void setup() {
  sensors.setup();
  fans.setup();
}

void loop() {
  sensors.update();
  fans.loop();

  lamp.on();
  delay(3000);
  lamp.off();
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