#ifndef HEATER_H
#define HEATER_H

Relay  heater     (16);
Relay  heaterfan  (18);

class Heater{

public:

  void on()  { heater.on (); heaterfan.on (); }
  void off() { heater.off(); heaterfan.off(); }
};

#endif