#ifndef RELAY_H
#define RELAY_H

class Relay{
  private:
  int RELAY_PIN;
  public:
  Relay(int pin){
    RELAY_PIN = pin;
    pinMode(pin, OUTPUT);
    off();
  }

  void on(){ digitalWrite(RELAY_PIN, LOW); Serial.println("Relay " + String(RELAY_PIN) + " ON!"); }
  void off(){ digitalWrite(RELAY_PIN, HIGH); Serial.println("Relay " + String(RELAY_PIN) + " OFF!");}
};

#endif





