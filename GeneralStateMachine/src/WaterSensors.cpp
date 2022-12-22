#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "SensorPh.h"
#include "SensorEc.h"
#include "SensorO2.h"
#include "WaterSensors.h"

//sampling
#define water_sampling 1000 //1secS

static float o2, ph,ec;
long int TimeStamp_o2;
long int TimeStamp_ph;
long int TimeStamp_ec;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

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
    timeClient.update();
}

void WaterSensors::water_sensors_tasks() //Executa a task de sampling em sampling
{
    ph = sensorph.get_ph();
    TimeStamp_ph = timeClient.getEpochTime();
    o2 = sensoro2.get_o2();
    TimeStamp_o2 = timeClient.getEpochTime();
    ec = sensorec.get_ec();
    TimeStamp_ec = timeClient.getEpochTime();
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

long int WaterSensors::return_TS_ph()
{
    return TimeStamp_ph;
}

long int WaterSensors::return_TS_o2()
{
    return TimeStamp_o2;
}

long int WaterSensors::return_TS_ec()
{
    return TimeStamp_ec;
}