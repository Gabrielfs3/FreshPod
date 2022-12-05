#include <Arduino.h>
#include "sensor_ph.h"
#include "sensor_ec.h"
#include "sensor_o2.h"
#include "water_sensors.h"

//sampling
#define water_sampling 1000 //1secS

static float o2, ph,ec;

water_sensors::water_sensors(int pin, int pinx, int piny)
{
    sensor_pin_ph = pin;
    sensor_pin_o2 = pinx;
    sensor_pin_ec = piny;
}

sensorpH sensorph(sensor_pin_ph);
sensorO2 sensoro2(sensor_pin_o2);
sensorEC sensorec(sensor_pin_ec);

void water_sensors::init()
{
    Serial.begin(115200);
    sensorph.init();
    sensoro2.init();
    sensorec.init();
}

void water_sensors::water_sensors_tasks() //Executa a task de sampling em sampling
{
    ph = sensorph.get_pH();
    o2 = sensoro2.get_O2();
    ec = sensorec.get_EC();
}

float water_sensors::return_pH()
{
    return ph;
}

float water_sensors::return_O2()
{
    return o2;
}

float water_sensors::return_EC()
{
    return ec;
}