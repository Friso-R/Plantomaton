#ifndef LEDGROUP_H
#define LEDGROUP_H

Relay powerSwitch (2);
Relay ledPomp     (17);
Relay lampFans    (4);

Relay lamp1 (13);
Relay lamp2 (18);
Relay lamp3 (5);

class LedGroup {
public:

    int ledGroup[3];

    void ledGroupOn(){
    powerSwitch.on();
    ledPomp.on();
    lampFans.on();

    lamp1.set(ledGroup[0]);
    lamp2.set(ledGroup[1]);
    lamp3.set(ledGroup[2]);
    }

    void ledGroupOff(){
    lamp1.off();
    lamp2.off();
    lamp3.off();

    lampFans.off();
    ledPomp.off();
    powerSwitch.off();
    }
};

#endif