#include "ADS1X15.h"

ADS1115 ADS(0x48);

class SoilMoisture{
public:

  int16_t v0;  
  int16_t v1;  
  int16_t v2;  
  int16_t v3;
  
  void setup() 
  {
    Serial.println(__FILE__);
    Serial.print("ADS1X15_LIB_VERSION: ");
    Serial.println(ADS1X15_LIB_VERSION);

    Wire.begin();
    ADS.begin();
  }

  void update() 
  {
    ADS.setGain(0);

    int16_t v0 = ADS.readADC(0);  
    int16_t v1 = ADS.readADC(1);  
    int16_t v2 = ADS.readADC(2);  
    int16_t v3 = ADS.readADC(3);  

    float f = ADS.toVoltage(1);  //  voltage factor

    Serial.print("\tAnalog0: "); Serial.print(v0); Serial.print('\t'); Serial.println(v0 * f, 3);
    Serial.print("\tAnalog1: "); Serial.print(v1); Serial.print('\t'); Serial.println(v1 * f, 3);
    Serial.print("\tAnalog2: "); Serial.print(v2); Serial.print('\t'); Serial.println(v2 * f, 3);
    Serial.print("\tAnalog3: "); Serial.print(v3); Serial.print('\t'); Serial.println(v3 * f, 3);
    Serial.println();
  }
}; 