#ifndef TMP_DS18B12_H
#define TMP_DS18B12_H

#include "DS18B20.h"

#define ONE_WIRE_BUS 25

OneWire oneWire(ONE_WIRE_BUS);
DS18B20 sensor(&oneWire);

class Tmp_DS18B12{
public:

  float tmp;

  void setup(void)
  {
    sensor.begin();
    sensor.setResolution(11);
  }

  void update()
  {
    sensor.requestTemperatures();

    //  wait until sensor is ready
    while (!sensor.isConversionComplete())  
    {   delay(1);  }

    tmp = sensor.getTempC();
    Serial.print("Temp: ");
    Serial.println(tmp);
  }

};

#endif