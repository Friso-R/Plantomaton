#include "Files.h"

WiFiSetup wifi;
Broker    broker; 
Sensors   sensors;
LedGroup  leds;

Fan    sideFans   (14);
Relay  pomp       (4);
Switch humidifier (26);

ServoMotor servo  (23);

float optimal[5];
bool scheduleMode;
int timeOn, timeOff;

BlockNot  update    (5, SECONDS);
BlockNot  flowTimer (2, SECONDS);

void setup() {
  Serial.begin(9600);
  wifi.connect();
  broker.begin(); 
  sensors.setup();
  broker.publish("status/kas", "online");
}

void loop() {
  
  broker.update();

  if(update.TRIGGERED){
    sensors.refresh();
    pubSensors();
    regulate();
    check_schedule();
  }
}

void regulate(){
  sideFans.set(fanControl());
  sensors.soil_3 > optimal[4] ? pomp.on() : pomp.off(); //3000 tot 1300
//sensors.waves[10]    < optimal[3] ? lamp.on() : lamp.off();
}

void check_schedule(){
  int now = wifi.nowTimeMin();
  if (now == timeOn)  leds.ledGroupOn ();
  if (now == timeOff) leds.ledGroupOff();
}

int schedule(String timeStr) {
  int h, m, s, timeMin;
  sscanf(timeStr.c_str(), "%d:%d:%d", &h, &m, &s);

  timeMin = h*60 + m;
  return timeMin;
}

int fanControl(){
  float tmp = sensors.temperature;
  float hum = sensors.humidity;

  if (tmp > optimal[1] || hum > optimal[3]) 
    return 255;
  if (tmp > optimal[0] || hum > optimal[2]) 
    return 160;

  return 0;
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
  broker.publish("tmp"  , String(sensors.temperature ));
  broker.publish("vocht", String(sensors.humidity    ));
//broker.publish("vpd"  , String(sensors.vpd         ));
  broker.publish("soil" , String(sensors.soil_3));
  broker.publish("CO2"  , String(sensors.eCO2        ));
/*
  broker.publish("F1", String(sensors.waves[0]));
  broker.publish("F2", String(sensors.waves[1]));
  broker.publish("F3", String(sensors.waves[2]));
  broker.publish("F4", String(sensors.waves[3]));
  broker.publish("F5", String(sensors.waves[6]));
  broker.publish("F6", String(sensors.waves[7]));
  broker.publish("F7", String(sensors.waves[8]));
  broker.publish("F8", String(sensors.waves[9]));
  */
}

// This function is executed when some device publishes a message to a topic that the ESP32 is subscribed to
void callback(String topic, byte* message, unsigned int length) {
  String msg;

  for (int i = 0; i < length; i++)  
    msg += (char)message[i];
    
  if(topic == "infob3it/student033/lamp/1") {msg == "on" ? lamp1.on() : lamp1.off();}
  if(topic == "infob3it/student033/lamp/2") {msg == "on" ? lamp2.on() : lamp2.off();}
  if(topic == "infob3it/student033/lamp/3") {msg == "on" ? lamp3.on() : lamp3.off();}

  if(topic == "infob3it/student033/sidefans") msg == "on" ? lampFans.on() : lampFans.off();

  if(topic == "infob3it/student033/schedule/on")   timeOn  = schedule(msg);
  if(topic == "infob3it/student033/schedule/off")  timeOff = schedule(msg);

  if(topic == "infob3it/student033/servo"){
    msg == "open" ? servo.Open() : servo.Close();
  }
  if(topic == "infob3it/student033/ledGroup/1"){
    leds.ledGroup[0] = msg.toInt(); }
  if(topic == "infob3it/student033/ledGroup/2"){
    leds.ledGroup[1] = msg.toInt(); }
  if(topic == "infob3it/student033/ledGroup/3"){
    leds.ledGroup[2] = msg.toInt(); }

  if(topic == "infob3it/student033/pomp"){
    if(msg == "on")  pomp.on();
    if(msg == "off") pomp.off();    
  }
  if(topic == "infob3it/student033/humi"){
    if(msg == "off"){
      humidifier.on();}
    if(msg == "on"){
      humidifier.off();  }
  }
  if(topic == "infob3it/student033/optimal")
    sscanf(msg.c_str(), "%f %f %f %f %f", &optimal[0], &optimal[1], &optimal[2], &optimal[3], &optimal[4]);
}
