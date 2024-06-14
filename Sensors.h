#ifndef SENSORS_H
#define SENSORS_H

#include "DHTsensor.h"
#include "RGBsensor.h"
#include "co2.h"
#include <BlockNot.h>

BlockNot refresh(5, SECONDS);

class Sensors {
public:
  float temperature;
  float humidity;
  uint16_t* wavelengths;

  Sensors() : rgb() {}  // Initialize RGB sensor object

  void processReadings(uint16_t* readings) {
    wavelengths = readings;
  }

  void setup() {
    humi.setup();
    rgb.setup();
    co2.setup();
  }

  void update(){
    refresh_all();
  }

  void refresh_all(){
    if(refresh.TRIGGERED){
        humi.loop();
        rgb.loop();
        co2.loop();
      
      temperature = humi.humidity;
      humidity = humi.temperature;
      wavelengths = rgb.readings;
      
      // Process RGB readings
      processReadings(rgb.readings);
    }
  }

private:
  DHT humi;
  RGB rgb;
  SGP co2;
  
};

#endif // SENSORS_H
