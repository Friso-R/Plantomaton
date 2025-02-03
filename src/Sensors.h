#ifndef SENSORS_H
#define SENSORS_H

#include "DHTsensor.h"
//#include "RGBsensor.h"
#include "co2.h"
#include "SoilMoisture.h"
#include "Flux.h"

class Sensors {
public:
  float temperature;
  float humidity;
  float lux;
  //float vpd;
  int soilMoisture;
  uint16_t* waves;
  uint16_t eCO2;

  //Sensors() : rgb() {}  // Initialize RGB sensor object

  void setup() {
    humi.setup();
    flux.setup();
    //rgb.setup();
    //co2.setup();
  }

  void refresh(){
    
    humi.loop();
    flux.update();
    //rgb.loop();
    //co2.loop();
  	
    temperature  = humi.t;
    humidity     = humi.h;
    lux          = flux.lux;
    //vpd          = humi.vpd;
    //waves        = rgb.readings;
    soilMoisture = sms.getSoilMoisture();
    eCO2         = co2.CO2;
    // Process RGB readings
    //processReadings(rgb.readings);
  }

private:
  DHT21xxx humi;
  //RGB rgb;
  Flux flux;
  SGP co2;
  SoilMoisture sms;
  
  void processReadings(uint16_t* readings) 
  { waves = readings; }
  
};


#endif // SENSORS_H
