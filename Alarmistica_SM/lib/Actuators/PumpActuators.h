#include <stdbool.h>
#include "Arduino.h"

#ifndef PumpActuators_h
#define PumpActuators_h

void Init_Pump();
void Pump_Relay(boolean value);
boolean is_RelayPumpOn();


#endif