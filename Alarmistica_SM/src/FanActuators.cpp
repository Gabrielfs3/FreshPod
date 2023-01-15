#include <Arduino.h>
#include <Actuators.h>
#include <stdbool.h>
#include <FanActuators.h>

const int FanPIN = 25;     //nr of output pin LED relay
boolean Fanstate = 0 ;         //variable to store state of LED relay, 1 = on

void Init_Fan()                 //set FansPin as Output
{
   pinMode(FanPIN,OUTPUT);
}

void Fan_Relay(boolean entrystate)     //set Fans Relay On/Off , 1 = on
{
    if(entrystate == 1)
    {
    digitalWrite(FanPIN,HIGH);
    Fanstate = 1;
    }
    else
    {
    digitalWrite(FanPIN,LOW);
    Fanstate =0;
    }
}

boolean is_RelayFanOn()            // returns state of Fans relay, 1 = on
{
    return Fanstate;
}



