#include <Arduino.h>
#include <stdbool.h>
#include <PumpActuators.h>

const int PumpPIN = 27;     //nr of output pin LED relay
boolean Pumpstate = 0 ;         //variable to store state of LED relay, 1 = on

void Init_Pump()                 //set LEDsPin as Output
{
   pinMode(PumpPIN,OUTPUT);
}

void Pump_Relay(boolean entrystate)     //set LEDs Relay On/Off , 1 = on
{

    if(entrystate == 1)
    {
    digitalWrite(PumpPIN,HIGH);
    Pumpstate = 1;
    }
    else
    {
    digitalWrite(PumpPIN,LOW);
    Pumpstate =0;
    }
}

boolean is_RelayPumpOn()            // returns state of LEDs relay, 1 = on
{
    return Pumpstate;
}



