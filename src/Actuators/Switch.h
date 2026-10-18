#pragma once

class Switch{

  int SWITCH_PIN;

public:

Switch(int pin = NO_PIN){ setup(pin); }

  void setup(int pin){
    SWITCH_PIN = pin;
    if (SWITCH_PIN != NO_PIN) { pinMode(SWITCH_PIN, OUTPUT); }
  }

  void on() { 
    if (SWITCH_PIN != NO_PIN) { digitalWrite(SWITCH_PIN, HIGH); Serial.println("Switch " + String(SWITCH_PIN) + " ON!"); }
  }
  void off() { 
    if (SWITCH_PIN != NO_PIN) { digitalWrite(SWITCH_PIN, LOW);  Serial.println("Switch " + String(SWITCH_PIN) + " OFF!"); }
  }
};