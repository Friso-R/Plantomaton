#include <Arduino.h>
#include <BlockNot.h>

#include "Relay.h"
#include "Switch.h"
#include "Fan.h"

int getThirstLevel();

#include "Pomp.h"
#include "LedGroup.h"
#include "WiFiSetup.h"
#include "Broker.h"
#include "Sensors.h"
//#include "ServoMotor.h"

void regulate();
void pubSensors();
void callback(String topic, byte* message, unsigned int length);
int schedule(String timeStr);
int fanControl();
void check_schedule();