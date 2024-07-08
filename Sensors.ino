#include "Broker.h"
#include "Sensors.h"
#include "Relay.h"
#include "SimButton.h"
#include <BlockNot.h>

Broker    broker; 
Sensors   sensors;

BlockNot  update    (5, SECONDS);
BlockNot  flowTimer (2, SECONDS);

SimButton humi(19);

Relay lamp    (33);
Relay lampFans(27);
Relay tmpFans (14);
Relay pomp    (12);

float optimal[5];

void setup() {
  Serial.begin(115200);
  broker.setup_wifi();
  broker.begin();
  sensors.setup();
  broker.publish("start", "start!!!");
}

void loop() {
  broker.update();
  sensors.update();

  if(update.TRIGGERED){
    pubSensors();
    regulate();
  }
}

void regulate(){
  sensors.temperature  > optimal[0] ? tmpFans.on() : tmpFans.off();
  sensors.soilMoisture > optimal[1] ? pomp.on()    : pomp.off(); //3000 tot 1300
  sensors.humidity     > optimal[2] ? tmpFans.on() : tmpFans.off();
  sensors.waves[10]    < optimal[3] ? lamp.on()    : lamp.off();
  if (sensors.humidity < optimal[2])
    humi.toggle(); 
}

void SupplyWater(){
  pomp.on();
  flowTimer.RESET;
} 

void BlockWater(){
  if(flowTimer.FIRST_TRIGGER)
    pomp.off();
}

