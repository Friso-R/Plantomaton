#include "Files.h"

WiFiSetup wifi;
Broker    broker; 
Sensors   sensors;
LedGroup  leds;

Pomp   pomp;
Fan    sideFans   (PIN_FAN_POWER, PIN_FAN_BOOST);
Heater heater     (PIN_HEATER, PIN_HEATER_FAN);
Switch humidifier (PIN_HUMIDIFIER);

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

// Standaardwaarden: 08:00 (480 min) tot 20:00 (1200 min)
int timeOn  = 8 * 60;  
int timeOff = 20 * 60;
bool isLampScheduledOn = false;

bool scheduleMode;

BlockNot  update        (5, SECONDS);
BlockNot  manualupdate  (1, SECONDS);

// FIX UIT KIEMKAS: Debug-functie om actuele instellingen in te zien
void printOptimalValues() {
  Serial.println("Current optimal values:");
  for (int i = 0; i < 10; i++) {
    Serial.print("optimal[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(optimal[i]);
  }
}

void setup() {
  // FIX UIT KIEMKAS: Baudrate verhoogd van 9600 naar 115200 om CPU-blocking te voorkomen
  Serial.begin(115200);
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

  sensors.humidity < optimal[5] ? humidifier.on() : humidifier.off();
  
  sensors.tmp_air  < optimal[6] ? heater.on() : heater.off();
}

void check_schedule(){
  int now = wifi.nowTimeMin();

  if (now < 0) return;

  bool shouldBeOn = false;

  if (timeOn < timeOff) {
    if (now >= timeOn && now < timeOff) {
      shouldBeOn = true;
    }
  } else if (timeOn > timeOff) {
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
  char msgBuffer[16]; 

  snprintf(msgBuffer, sizeof(msgBuffer), "%.2f", sensors.tmp_air);
  broker.publish("tmp/air", msgBuffer);

  snprintf(msgBuffer, sizeof(msgBuffer), "%.2f", sensors.tmp_lamp);
  broker.publish("tmp/soil", msgBuffer);

  snprintf(msgBuffer, sizeof(msgBuffer), "%.2f", sensors.humidity);
  broker.publish("vocht", msgBuffer);

  snprintf(msgBuffer, sizeof(msgBuffer), "%.2f", sensors.lux);
  broker.publish("lux", msgBuffer);

  snprintf(msgBuffer, sizeof(msgBuffer), "%d", sensors.soil_1);
  broker.publish("soil", msgBuffer);
}

void callback(String topic, byte* message, unsigned int length) {
  topic = topic.substring(4);
  
  // FIX UIT KIEMKAS: Stack-buffer met memcpy voorkomt heap-fragmentatie
  char msgBuffer[length + 1];
  memcpy(msgBuffer, message, length);
  msgBuffer[length] = '\0';
  String msg = String(msgBuffer);
    
  if(topic == "sidefans") sideFans.rotation_speed(msg.toInt());

  if(topic == "schedule/on")   timeOn  = schedule(msg);
  if(topic == "schedule/off")  timeOff = schedule(msg);

  if(topic == "ledGroup/switch") { msg == "on" ?  leds.ledGroupOn() : leds.ledGroupOff(); }

  if(topic == "ledGroup/1") { leds.ledGroup[0] = msg.toInt(); }

if(topic == "pomp"){
    if(msg == "on")  pomp.supplyWater();
    else if(msg == "off") pomp.turnOff();                
    else pomp.setDuration(msg.toInt());                   
  }
  if(topic == "humi"){
    if(msg == "on")  { humidifier.on();   }
    if(msg == "off") { humidifier.off();  }
  }
  
if(topic == "optimal"){
    int i; 
    float val;
    sscanf(msg.c_str(), "%d %f", &i, &val); 
    if (i >= 0 && i < 10) {  // <-- Safety Check
      optimal[i] = val;
      printOptimalValues();
    }
  }
}

int getThirstLevel(){
  int soil = sensors.soil_1;
  return (optimal[4] - soil);
}