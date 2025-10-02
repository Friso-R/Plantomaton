#ifndef FAN_H
#define FAN_H

class Fan{

  int POWER_PIN;
  int LEVEL_PIN;

public:

  Fan(int power_pin, int level_pin){
    POWER_PIN = power_pin;
    LEVEL_PIN = level_pin;

    pinMode(POWER_PIN, OUTPUT);
    pinMode(LEVEL_PIN, OUTPUT);
  }

  void on ()            { digitalWrite(POWER_PIN, HIGH);       Serial.println("Fan on "); }
  void off()            { digitalWrite(POWER_PIN, LOW );       Serial.println("Fan off"); }
  void level(int level) { digitalWrite(LEVEL_PIN,  level - 1); Serial.println("Fan level:" + String(level)); on();}

};

#endif