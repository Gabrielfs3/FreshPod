#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "SensorPh.h"
#include "SensorEc.h"
#include "SensorO2.h"
#include "WaterSensors.h"
#include "HC_SR04_driver.h"

//sampling
#define water_sampling 1000 //1secS

static float o2, ph, ec, water_level;
long int TimeStamp_o2;
long int TimeStamp_ph;
long int TimeStamp_ec;
long int TimeStamp_water_level;
int pinecho, pintrig;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

WaterSensors::WaterSensors(int pin, int pinx, int piny, int pink, int ping)
{
    sensor_pin_ph = pin;
    sensor_pin_o2 = pinx;
    sensor_pin_ec = piny;
    echo_Pin = pink;
    trig_Pin = ping;
    pinecho = echo_Pin;
    pintrig = trig_Pin;
}

SensorPh sensorph(sensor_pin_ph);
SensorO2 sensoro2(sensor_pin_o2);
SensorEc sensorec(sensor_pin_ec);
HC_SR04_driver sensorwl(pinecho,pintrig);

void WaterSensors::init()
{
    Serial.begin(115200);
    sensorph.init();
    sensoro2.init();
    sensorec.init();
    sensorwl.Init();
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
    water_level = sensorwl.get_dist();
    TimeStamp_water_level = timeClient.getEpochTime();
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

float WaterSensors::return_water_level()
{
    return water_level;
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

long int WaterSensors::return_TS_water_level()
{
    return TimeStamp_water_level;
}