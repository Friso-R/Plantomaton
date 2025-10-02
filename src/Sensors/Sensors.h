#ifndef SENSORS_H
#define SENSORS_H

#include "Tmp_DS18B20.h"
//#include "RGBsensor.h"
//#include "co2.h"
#include "SoilMoisture.h"
#include "Flux.h"
#include "Humidity.h"

class Sensors {
public:
  float tmp_air;
  float tmp_lamp;
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
    
    tmp.setup();
    flux.setup();
    sht.setup();
    //rgb.setup();
    //co2.setup();
  }

  void refresh(){
    
    tmp.update();
    flux.update();
    sms.update();
    sht.update();
    //rgb.loop();
    //co2.loop();
  	
    tmp_lamp	   = tmp.tmp;
    tmp_air      = sht.aTemperature;
    humidity     = sht.aHumidity;
    lux          = flux.lux;
    
  //vpd          = humi.vpd;
  //waves        = rgb.readings;
  //eCO2         = co2.CO2;

    soil_0       = sms.v0;
    soil_1       = sms.v1;
    soil_2       = sms.v2;
    soil_3       = sms.v3;

    // Process RGB readings
    //processReadings(rgb.readings);
  }

private:
  Tmp_DS18B12  tmp;
  //RGB        rgb;
  Flux         flux;
  //SGP          co2;
  SoilMoisture sms;
  SHT40        sht;
  
  void processReadings(uint16_t* readings) 
  { waves = readings; }
  
};


#endif // SENSORS_H
