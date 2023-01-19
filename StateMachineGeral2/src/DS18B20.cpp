#include <Arduino.h>
#include "DS18B20.h"
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(32);                    // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);    // Pass our oneWire reference to Dallas Temperature sensor 

DS18B20::DS18B20( int pin)
{
    pin_temp=pin;
}

void DS18B20::Init()
{
    sensors.begin();
}

void DS18B20::update_temperature()
{
    sensors.requestTemperatures(); 
    temperature = sensors.getTempCByIndex(0);
}

float DS18B20::get_temperature()
{
    return temperature;                 //return temperature
}