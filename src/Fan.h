#ifndef FAN_H
#define FAN_H

class Fan{

  int FAN_PIN;

public:

  Fan(int pin){
    FAN_PIN = pin;
    pinMode(pin, OUTPUT);
    off();
  }

  void set(int pwm) { analogWrite(FAN_PIN,  pwm); Serial.println("Fan " + String(FAN_PIN) + " ON!"); }
  void off(){ digitalWrite(FAN_PIN, LOW); Serial.println("Fan " + String(FAN_PIN) + " OFF!");}
};

#endif