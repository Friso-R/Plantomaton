#include <Arduino.h>
#include <BlockNot.h>

#include "WiFiSetup.h"
#include "Broker.h"
#include "Sensors.h"
#include "Relay.h"
#include "SimButton.h"
#include "ServoMotor.h"

WiFiSetup wifi;
Broker    broker; 
Sensors   sensors;

BlockNot  update    (5, SECONDS);
BlockNot  flowTimer (2, SECONDS);

SimButton  humi  (18);
ServoMotor servo (17);

Relay lamp1   (33);
Relay lamp2   (33);
Relay lamp3   (33);

Relay lampFans(27);
Relay tmpFans (14);
Relay pomp    (12);

float optimal[5];

void regulate();
void SupplyWater();
void BlockWater();
void pubSensors();
void callback(String topic, byte* message, unsigned int length);

void setup() {
  Serial.begin(9600);
  wifi.connect();
  broker.begin();
  sensors.setup();
  broker.publish("status/kas", "online");
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
  //sensors.waves[10]    < optimal[3] ? lamp.on()    : lamp.off();
  if (sensors.humidity < optimal[2])  humi.toggle(); 
}

void SupplyWater(){
  pomp.on();
  flowTimer.RESET;
} 

void BlockWater(){
  if(flowTimer.FIRST_TRIGGER)
    pomp.off();
}

void pubSensors(){
  broker.publish("tmp",   String(sensors.temperature));
  broker.publish("vocht", String(sensors.humidity));
  //broker.publish("vpd",   String(sensors.vpd));
  broker.publish("soil",  String(sensors.soilMoisture));

  broker.publish("F1", String(sensors.waves[0]));
  broker.publish("F2", String(sensors.waves[1]));
  broker.publish("F3", String(sensors.waves[2]));
  broker.publish("F4", String(sensors.waves[3]));
  broker.publish("F5", String(sensors.waves[6]));
  broker.publish("F6", String(sensors.waves[7]));
  broker.publish("F7", String(sensors.waves[8]));
  broker.publish("F8", String(sensors.waves[9]));
}

// This function is executed when some device publishes a message to a topic that the ESP32 is subscribed to
void callback(String topic, byte* message, unsigned int length) {
  String msg;

  for (int i = 0; i < length; i++)  
    msg += (char)message[i];
    
  if(topic == "infob3it/student033/lamp/1") {msg == "on" ? lamp1.on() : lamp1.off();}
  if(topic == "infob3it/student033/lamp/2") {msg == "on" ? lamp2.on() : lamp2.off();}
  if(topic == "infob3it/student033/lamp/3") {msg == "on" ? lamp3.on() : lamp3.off();}

  if(topic == "infob3it/student033/lampfans") msg == "on" ? lampFans.on() : lampFans.off();

  if(topic == "infob3it/student033/servo"){
    if(msg == "open"){
      servo.Open();}
  if(msg == "close"){
      servo.Close();  }
  }
  if(topic == "infob3it/student033/pomp"){
    if(msg == "on")  pomp.on();
    if(msg == "off") pomp.off();    
  }
  if(topic == "infob3it/student033/humi")
    if(msg == "toggle") humi.toggle();
  if(topic == "infob3it/student033/optimal")
    sscanf(msg.c_str(), "%f %f %f %f %f", &optimal[0], &optimal[1], &optimal[2], &optimal[3], &optimal[4]);
}
