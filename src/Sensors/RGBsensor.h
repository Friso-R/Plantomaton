#pragma once

#include <Adafruit_AS7341.h>

Adafruit_AS7341 as7341;

class RGB {
public:
  uint16_t readings[12];

  void setup() {
    if (!as7341.begin()) {
      Serial.println("Could not find AS7341");
      return;
    }
    
    as7341.setATIME(100);
    as7341.setASTEP(999);
    as7341.setGain(AS7341_GAIN_256X);
  }

  void loop() {
    if (!as7341.readAllChannels(readings)) {
      Serial.println("RGB sensor - Error reading all channels!");
      return;
    }
    
    Serial.print("ADC0/F1 415nm : ");
    Serial.println(readings[0]);
    Serial.print("ADC1/F2 445nm : ");
    Serial.println(readings[1]);
    Serial.print("ADC2/F3 480nm : ");
    Serial.println(readings[2]);
    Serial.print("ADC3/F4 515nm : ");
    Serial.println(readings[3]);
    Serial.print("ADC0/F5 555nm : ");
    Serial.println(readings[6]);
    Serial.print("ADC1/F6 590nm : ");
    Serial.println(readings[7]);
    Serial.print("ADC2/F7 630nm : ");
    Serial.println(readings[8]);
    Serial.print("ADC3/F8 680nm : ");
    Serial.println(readings[9]);
    Serial.print("ADC4/Clear    : ");
    Serial.println(readings[10]);
    Serial.print("ADC5/NIR      : ");
    Serial.println(readings[11]);

    Serial.println();
    
  }
};
