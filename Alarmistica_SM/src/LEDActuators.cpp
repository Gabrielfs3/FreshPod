#include <Arduino.h>
#include <stdbool.h>
#include <LEDActuators.h>

const int LEDPIN = 26;     //nr of output pin LED relay
boolean LEDstate = 0 ;         //variable to store state of LED relay, 1 = on

void Init_LED()                 //set LEDsPin as Output
{
   pinMode(LEDPIN,OUTPUT);
}

void LED_Relay(boolean entrystate)     //set LEDs Relay On/Off , 1 = on
{
    if(entrystate == 1)
    {
    digitalWrite(LEDPIN,HIGH);
    LEDstate = 1;
    }
    else
    {
    digitalWrite(LEDPIN,LOW);
    LEDstate =0;
    }
}

boolean is_RelayLEDOn()            // returns state of LEDs relay, 1 = on
{
    return LEDstate;
}



