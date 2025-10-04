#pragma once

Switch power;
Switch boost;

class Fan{ 


public:
  
  Fan(int power_pin, int level_pin){
    power.setup(power_pin);
    boost.setup(level_pin);
    
  }

  // Set fan speed level: 0 = off, 1 = low, 2 = high
  void rotation_speed(int level) { 
    switch (level){
    case 0:
      power.off();
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