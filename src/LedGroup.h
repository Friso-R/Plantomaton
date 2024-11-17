#ifndef LEDGROUP_H
#define LEDGROUP_H

Relay lamp1(33);
Relay lamp2(25);
Relay lamp3(26);

Relay lampFans(27);

class LedGroup {
public:

    bool ledGroup [3];

    void ledGroupOn(){
    lampFans.on();
    if(ledGroup[0]) lamp1.on();
    if(ledGroup[1]) lamp2.on();
    if(ledGroup[2]) lamp3.on();
    }

    void ledGroupOff(){
    lamp1.off();
    lamp2.off();
    lamp3.off();
    lampFans.off();
    }
};

#endif