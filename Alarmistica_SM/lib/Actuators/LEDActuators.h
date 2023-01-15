#include <stdbool.h>
#include "Arduino.h"

#ifndef LEDActuators_h
#define LEDActuators_h

void Init_LED();           
void LED_Relay(boolean value);
boolean is_RelayLEDOn();


#endif