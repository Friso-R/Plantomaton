#pragma once

Relay powerSwitch (32);
Relay ledPomp     (19);
Relay lampFans    (5);

LedPWM lamp1 (13, 0);
//LedPWM lamp2 (18);
//LedPWM lamp3 (5);

class LedGroup {
public:

    int ledGroup[3];

    void ledGroupOn(){
    powerSwitch.off();
    ledPomp.on();
    lampFans.on();

    lamp1.set(ledGroup[0]);
    //lamp2.set(ledGroup[1]);
    //lamp3.set(ledGroup[2]);
    }

    void ledGroupOff(){
    lamp1.off();
    //lamp2.off();
    //lamp3.off();

    lampFans.off();
    ledPomp.off();
    powerSwitch.on();
    }
};