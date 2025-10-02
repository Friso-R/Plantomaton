#ifndef LEDPWM_H
#define LEDPWM_H

class LedPWM{

  int LED_PIN;
  int LED_CHANNEL;

public:

  LedPWM(int pin, int channel, int freq = 10000, int resolution = 8){
    LED_PIN = pin;
    LED_CHANNEL = channel;
    ledcSetup(channel, freq, resolution);
    ledcAttachPin(pin, channel);

    ledcWrite(channel, 0);
  }
  
  void set(int pwm) { ledcWrite(LED_CHANNEL,  pwm); Serial.println("LED " + String(LED_PIN) + " ON!" ); }
  void off()        { ledcWrite(LED_CHANNEL,  0);   Serial.println("LED " + String(LED_PIN) + " OFF!"); }
};

#endif





