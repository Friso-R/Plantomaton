#include "Files.h"

WiFiSetup wifi;
Broker    broker; 
Sensors   sensors;
LedGroup  leds;

Fan    sideFans   (27);
Relay  pomp       (15);
Relay  heater     (16);
Switch humidifier (19);

float optimal[10] = { 
//val   i actu    factor      
  80, //0 fan     air tmp     lower
  80, //1 fan     air tmp     upper
  80, //2 fan     humidity    lower
  80, //3 fan     humidity    upper
  80, //4 pomp    soil moisture
  18, //5 humi    humidity
  80, //6 heater  temperature 
  80, //7 
  80, //8
  80  //9
  };

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
//sensors.waves[10]    < optimal[] ? lamp.on() : lamp.off();
  sensors.humidity < optimal[5]  ? humidifier.on() : humidifier.off();
  sensors.tmp_air < optimal[6]  ? heater.on() : heater.off();

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
  float tmp = sensors.tmp_air;
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
  broker.publish("tmp/air"  , String(sensors.tmp_air ));
  broker.publish("tmp/soil" , String(sensors.tmp_soil));
  broker.publish("vocht"    , String(sensors.humidity));
  broker.publish("lux"      , String(sensors.lux     ));
//broker.publish("vpd"      , String(sensors.vpd     ));
  broker.publish("soil"     , String(sensors.soil_3  ));
//broker.publish("CO2"      , String(sensors.eCO2    ));
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
  topic = topic.substring(4);
  String msg;

  for (int i = 0; i < length; i++)  
    msg += (char)message[i];
    
  if(topic == "sidefans") msg == "on" ? sideFans.set(255) : sideFans.off();

  if(topic == "schedule/on")   timeOn  = schedule(msg);
  if(topic == "schedule/off")  timeOff = schedule(msg);

  if(topic == "ledGroup/switch") 
    { msg == "on" ?  leds.ledGroupOn() : leds.ledGroupOff(); }

  if(topic == "ledGroup/1") { leds.ledGroup[0] = msg.toInt(); }
  if(topic == "ledGroup/2") { leds.ledGroup[1] = msg.toInt(); }
  if(topic == "ledGroup/3") { leds.ledGroup[2] = msg.toInt(); }


  if(topic == "pomp"){
    if(msg == "on")  pomp.on();
    if(msg == "off") pomp.off();    
  }
  if(topic == "humi"){
    if(msg == "on")  { humidifier.on();   }
    if(msg == "off") { humidifier.off();  }
  }
  if(topic == "optimal"){
    int i, val;
    sscanf(msg.c_str(), "%d %d", &i, &val); 
    optimal[i] = val;
  }
}
