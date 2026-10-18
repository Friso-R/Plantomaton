#pragma once

class LedPWM {
  int LED_PIN;
  int LED_CHANNEL;
  int current_pwm = -1; // Tracks current PWM value to prevent spam (-1 forces first update)

public:
  LedPWM(int pin, int channel, int freq = 100, int resolution = 8) {
    LED_PIN = pin;
    LED_CHANNEL = channel;
    ledcSetup(channel, freq, resolution);
    ledcAttachPin(pin, channel);

    off(); // Initialize hardware to 0
  }
  
  void set(int pwm) { 
    if (current_pwm != pwm) {
      ledcWrite(LED_CHANNEL, pwm); 
      current_pwm = pwm;
      Serial.printf("LED %d set to %d\n", LED_PIN, pwm); 
    }
  }
  
  void off() { 
    if (current_pwm != 0) {
      ledcWrite(LED_CHANNEL, 0);   
      current_pwm = 0;
      Serial.printf("LED %d OFF!\n", LED_PIN); 
    }
  }
};