#ifndef FLUX_H
#define FLUX_H

/*

Advanced BH1750 library usage example

This example has some comments about advanced usage features.

Connections

  - VCC to 3V3 or 5V
  - GND to GND
  - SCL to SCL
  - SDA to SDA
  - ADD to (not connected) or GND

ADD pin is used to set sensor I2C address. If it has voltage greater or equal
to 0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address
will be 0x23 (by default).

*/

#include <BH1750.h>

/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.

*/

BH1750 lightMeter(0x23);

class Flux{
public:

float lux;

  void setup() {

    Wire.begin();

    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
      Serial.println(F("BH1750 Advanced begin"));
    } else {
      Serial.println(F("Error initialising BH1750"));
    }
  }

  void update() {
    if (lightMeter.measurementReady()) {
      lux = lightMeter.readLightLevel();
      Serial.print(F("Light: "));
      Serial.print(lux);
      Serial.println(F(" lx"));

      if (lux < 0) {
        Serial.println(F("Error condition detected"));
      } else {
        if (lux > 40000.0) {
          // reduce measurement time - needed in direct sun light
          if (lightMeter.setMTreg(32)) {
            Serial.println(
                F("Setting MTReg to low value for high light environment"));
          } else {
            Serial.println(
                F("Error setting MTReg to low value for high light environment"));
          }
        } else {
          if (lux > 10.0) {
            // typical light environment
            if (lightMeter.setMTreg(69)) {
              Serial.println(F(
                  "Setting MTReg to default value for normal light environment"));
            } else {
              Serial.println(F("Error setting MTReg to default value for normal "
                              "light environment"));
            }
          } else {
            if (lux <= 10.0) {
              // very low light environment
              if (lightMeter.setMTreg(138)) {
                Serial.println(
                    F("Setting MTReg to high value for low light environment"));
              } else {
                Serial.println(F("Error setting MTReg to high value for low "
                                "light environment"));
              }
            }
          }
        }
      }
      Serial.println(F("--------------------------------------"));
    }
  }
};

#endif