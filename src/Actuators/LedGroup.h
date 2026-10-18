#pragma once
#include "Relay.h"
#include "LedPWM.h"

class LedGroup {
private:
    Relay powerSwitch{PIN_LED_POWER};
    Relay ledPomp{PIN_LED_PUMP};
    Relay lampFans{PIN_LED_FANS};
    LedPWM lamp1{PIN_LED_LAMP1, CHANNEL_LED_LAMP1};

public:
    int ledGroup[3];

    void ledGroupOn(){
        powerSwitch.off();
        ledPomp.on();
        lampFans.on();
        lamp1.set(ledGroup[0]);
    }

    void ledGroupOff(){
        lamp1.off();
        lampFans.off();
        ledPomp.off();
        powerSwitch.on();
    }
};