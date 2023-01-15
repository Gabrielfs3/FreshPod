#include <Arduino.h>
#include <Actuators.h>
#include <LEDActuators.h>
#include <FanActuators.h>
#include <PumpActuators.h>

void Init_AllActuators()
{
    Init_LED();
    Init_Fan();
    Init_Pump();
}

void setPump(boolean value)
{
 Pump_Relay(value);
}

void setLED(boolean value)
{
 LED_Relay(value);
}

void setFan(boolean value)
{
 Fan_Relay(value);
}


boolean Return_LED()
{
    boolean LEDstate;
    LEDstate = is_RelayLEDOn();
    return LEDstate;
}

boolean Return_Fan()
{
    boolean Fansstate;
    Fansstate = is_RelayFanOn();
    return Fansstate;
}

boolean Return_Pump()
{
    boolean Pumpstate;
    Pumpstate = is_RelayPumpOn();
    return Pumpstate;
}