#ifndef LEDPWM_H
#define LEDPWM_H

class LedPWM{

  int LED_PIN;

public:

  LedPWM(int pin, int channel, int freq = 5000, int resolution = 8){
    LED_PIN = pin;
    ledcSetup(channel, freq, resolution);
    ledcAttachPin(pin, channel);

    ledcWrite(channel, 0);
  }
  
  void set(int pwm) { analogWrite(LED_PIN,  pwm); Serial.println("LED " + String(LED_PIN) + " ON!" ); }
  void off()        { analogWrite(LED_PIN,  0);   Serial.println("LED " + String(LED_PIN) + " OFF!"); }
};

#endif





