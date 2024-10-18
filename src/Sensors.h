#ifndef SENSORS_H
#define SENSORS_H

#include "DHTsensor.h"
#include "RGBsensor.h"
#include "co2.h"
#include "SoilMoisture.h"

BlockNot refresh(5, SECONDS);

class Sensors {
public:
  float temperature;
  float humidity;
  //float vpd;
  int soilMoisture;
  uint16_t* waves;
  uint16_t eCO2;

  Sensors() : rgb() {}  // Initialize RGB sensor object

  void processReadings(uint16_t* readings) 
  { waves = readings; }

  void setup() {
    humi.setup();
    rgb.setup();
    co2.setup();
  }

  void update(){
    if(refresh.TRIGGERED)
      refresh_all();
  }

  void refresh_all(){
    
    humi.loop();
    rgb.loop();
    co2.loop();
  	
  
    temperature  = humi.h;
    humidity     = humi.t;
    //vpd          = humi.vpd;
    waves        = rgb.readings;
    soilMoisture = sms.getSoilMoisture();
    eCO2         = co2.CO2;
    // Process RGB readings
    processReadings(rgb.readings);
  
  }

private:
  DHT21xxx humi;
  RGB rgb;
  SGP co2;
  SoilMoisture sms;
  
};

#endif // SENSORS_H
