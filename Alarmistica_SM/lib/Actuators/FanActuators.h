#include <stdbool.h>
#include "Arduino.h"

#ifndef FanActuators_h
#define FanActuators_h

void Init_Fan();
void Fan_Relay(boolean value);
boolean is_RelayFanOn();


#endif