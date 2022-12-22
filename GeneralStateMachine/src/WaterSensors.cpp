#include <Arduino.h>
#include "SensorPh.h"
#include "SensorEc.h"
#include "SensorO2.h"
#include "WaterSensors.h"

//sampling
#define water_sampling 1000 //1secS

static float o2, ph,ec;

WaterSensors::WaterSensors(int pin, int pinx, int piny)
{
    sensor_pin_ph = pin;
    sensor_pin_o2 = pinx;
    sensor_pin_ec = piny;
}

SensorPh sensorph(sensor_pin_ph);
SensorO2 sensoro2(sensor_pin_o2);
SensorEc sensorec(sensor_pin_ec);

void WaterSensors::init()
{
    Serial.begin(115200);
    sensorph.init();
    sensoro2.init();
    sensorec.init();
}

void WaterSensors::water_sensors_tasks() //Executa a task de sampling em sampling
{
    ph = sensorph.get_ph();
    o2 = sensoro2.get_o2();
    ec = sensorec.get_ec();
}

float WaterSensors::return_ph()
{
    return ph;
}

float WaterSensors::return_o2()
{
    return o2;
}

float WaterSensors::return_ec()
{
    return ec;
}