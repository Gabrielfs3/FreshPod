#include <Arduino.h>
#include <Wire.h>
#include "..\lib\EnvironmentSensors\LuminositySensor.h"
              
//global variables
float luminosity=0.0;

LuminositySensor::LuminositySensor(int _SCL_, int _SCA_)
{
    _SCL=_SCL_;
    _SCA=_SCA_;
}

void LuminositySensor::Init()
{   
    //###########[ debug prints ]#########################
    Serial.begin(115200); // For testing
    Serial.print("GPIO's : "); Serial.print(_SCL); Serial.print(" ; "); Serial.println(_SCA);
    //####################################################
}

float LuminositySensor::get_luminosity()
{
    luminosity= random(1,65)/(10.0); //while we don't have the sensor
    return luminosity;
}
