#pragma once
#include "Relay.h"

class Fan { 
private:
  Relay power;
  Relay boost;

public:
  Fan(int power_pin, int level_pin){
    power.setup(power_pin);
    boost.setup(level_pin);
  }

  void rotation_speed(int level) { 
    switch (level){
    case 0:
      power.off();
      boost.off();
      break;
    case 1:
      power.on();
      boost.off();
      break;
    case 2:
      power.on();
      boost.on();
      break;
    }
  }
};