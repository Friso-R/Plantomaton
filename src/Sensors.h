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
  
  uint16_t* waves;
  uint16_t eCO2;

  int16_t soil_0;  
  int16_t soil_1;  
  int16_t soil_2;  
  int16_t soil_3;

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
    sms.update();
    //rgb.loop();
    //co2.loop();
  	
    temperature  = humi.t;
    humidity     = humi.h;
    lux          = flux.lux;
    //vpd          = humi.vpd;
    //waves        = rgb.readings;
    eCO2         = co2.CO2;

    soil_0       = sms.v0;
    soil_1       = sms.v1;
    soil_2       = sms.v2;
    soil_3       = sms.v3;

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
