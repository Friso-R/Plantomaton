#ifndef RELAY_H
#define RELAY_H

class Relay{
  private:
  int RELAY_PIN;
  public:
  Relay(int pin){
    RELAY_PIN = pin;
  }

  void on(){ digitalWrite(RELAY_PIN, HIGH); }
  void off(){ digitalWrite(RELAY_PIN, LOW); }
};

#endif





