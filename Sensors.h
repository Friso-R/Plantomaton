#ifndef SENSORS_H
#define SENSORS_H

#include "DHTsensor.h"
#include "RGBsensor.h"
#include "co2.h"
#include "SoilMoisture.h"
#include <BlockNot.h>

BlockNot refresh(5, SECONDS);

class Sensors {
public:
  float temperature;
  float humidity;
  float vpd;
  int soilMoisture;
  uint16_t* wavelengths;

  Sensors() : rgb() {}  // Initialize RGB sensor object

  void processReadings(uint16_t* readings) 
  { wavelengths = readings; }

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
  	
  
    temperature  = humi.humidity;
    humidity     = humi.temperature;
    vpd          = humi.vpd;
    wavelengths  = rgb.readings;
    soilMoisture = sms.getSoilMoisture();
    
    // Process RGB readings
    processReadings(rgb.readings);
  
  }

private:
  DHT humi;
  RGB rgb;
  SGP co2;
  SoilMoisture sms;
  
};

#endif // SENSORS_H
