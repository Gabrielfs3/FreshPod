#include <Arduino.h>

#include "../lib/WATER/sensor_ph.h"
#include "../lib/WATER/sensor_ec.h"
#include "../lib/WATER/sensor_o2.h"
#include "../lib/WATER/water_sensors.h"
#include "../lib/WATER/Water_Sensors_SM.h"



int sensor_pin_ph = 33;
int sensor_pin_o2= 32;
int sensor_pin_ec = 35;



void setup(){

    InitWaterSensors();
}



void loop() 
{

   WaterSensorsTasks();

}