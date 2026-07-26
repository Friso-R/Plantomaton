#include "Files.h"

WiFiSetup wifi;
Broker    broker; 
Sensors   sensors;
LedGroup  leds;

Pomp   pomp;
Fan    sideFans   (16, 17);
Heater heater     (-1, -1);
Switch humidifier (-1);

float optimal[10] = { 
//val   i actu    factor      
  28, //0 fan     air tmp     lower
  32, //1 fan     air tmp     upper
  80, //2 fan     humidity    lower
  95, //3 fan     humidity    upper
  40, //4 pomp    soil moisture
  40, //5 humi    humidity
  16, //6 heater  temperature 
  80, //7 
  80, //8
  80  //9
  };

// Standaardwaarden, bijvoorbeeld 08:00 (480 min) tot 20:00 (1200 min)
int timeOn  = 8 * 60;  
int timeOff = 20 * 60;
bool isLampScheduledOn = false; // Houdt bij wat de huidige status is


bool scheduleMode;


BlockNot  update        (5, SECONDS);
BlockNot  manualupdate  (1, SECONDS);

void setup() {
  Serial.begin(9600);
  wifi.setup();
  broker.begin(); 
  sensors.setup();
  broker.publish("status/kas", "online");
}

void loop() {
  wifi.handleTime();
  broker.handleConnection(); 
  broker.update();

  pomp.update();

  if(update.TRIGGERED){
    sensors.refresh();
    regulate();
    check_schedule();

    if (client.connected()) {
      pubSensors();
    }

  }

  if(manualupdate.TRIGGERED){
    
  }

}

void regulate(){
  sideFans.rotation_speed(fanControl());
  

  sensors.humidity < optimal[5] ? humidifier.off() : humidifier.on();
  sensors.tmp_air  < optimal[6] ? heater.on()      : heater.off();
  sensors.tmp_air  < optimal[6] ? heaterfan.on()   : heaterfan.off();
}

void check_schedule(){
  int now = wifi.nowTimeMin();

  if (now < 0) return;

  bool shouldBeOn = false;

  if (timeOn < timeOff) {
    // Normaal schema overdag (bijv. 08:00 tot 20:00)
    if (now >= timeOn && now < timeOff) {
      shouldBeOn = true;
    }
  } else if (timeOn > timeOff) {
    // Nachtschema dat over middernacht gaat (bijv. 22:00 tot 06:00)
    if (now >= timeOn || now < timeOff) {
      shouldBeOn = true;
    }
  }


  if (shouldBeOn && !isLampScheduledOn) {
    leds.ledGroupOn();
    isLampScheduledOn = true;
    Serial.println("Tijdschema geactiveerd: Lampen AAN");
  } else if (!shouldBeOn && isLampScheduledOn) {
    leds.ledGroupOff();
    isLampScheduledOn = false;
    Serial.println("Tijdschema gepauzeerd: Lampen UIT");
  }
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

  if (tmp < optimal[0] || hum < optimal[2]) 
    return 0;
  if (tmp < optimal[1] || hum < optimal[3]) 
    return 1;
  else 
    return 2;
}

void pubSensors(){
  broker.publish("tmp/air"  , String(sensors.tmp_air ));
  broker.publish("tmp/soil" , String(sensors.tmp_lamp));
  broker.publish("vocht"    , String(sensors.humidity));
  broker.publish("lux"      , String(sensors.lux     ));
//broker.publish("vpd"      , String(sensors.vpd     ));
  broker.publish("soil"     , String(sensors.soil_1  ));
//broker.publish("CO2"      , String(sensors.eCO2    ));

}

// This function is executed when some device publishes a message to a topic that the ESP32 is subscribed to
void callback(String topic, byte* message, unsigned int length) {
  topic = topic.substring(4);
  String msg;

  for (int i = 0; i < length; i++)  
    msg += (char)message[i];
    
  if(topic == "sidefans") sideFans.rotation_speed(msg.toInt());

  if(topic == "schedule/on")   timeOn  = schedule(msg);
  if(topic == "schedule/off")  timeOff = schedule(msg);

  if(topic == "ledGroup/switch") { msg == "on" ?  leds.ledGroupOn() : leds.ledGroupOff(); }

  if(topic == "ledGroup/1") { leds.ledGroup[0] = msg.toInt(); }
  //if(topic == "ledGroup/2") { leds.ledGroup[1] = msg.toInt(); }
  //if(topic == "ledGroup/3") { leds.ledGroup[2] = msg.toInt(); }

  if(topic == "pomp"){
    if(msg == "on")  pomp.supplyWater();
    else if(msg == "off") waterpump.off();
    else flowTimer.setDuration(msg.toInt());  
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

int getThirstLevel(){
  int soil = sensors.soil_1;
  return (optimal[4] - soil);
}
