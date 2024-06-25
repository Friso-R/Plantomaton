class SimButton {
  private:
    int RELAY_PIN;
  public:
    SimButton(int pin) {
      RELAY_PIN = pin;
      pinMode(pin, OUTPUT);
    }

    void toggle() {
      digitalWrite(RELAY_PIN, HIGH);
      delay(300);             
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("Button " + String(RELAY_PIN) + " toggled!");
    }
};


