#ifndef Water_Sensors_SM_h
#define Water_Sensors_SM_h

#include "../lib/WATER/water_sensors.h"

void WATER_SENSORS_INIT();
int runSwitchCaseWater(int timeMs);
void WaterSensorsTasks();
int returnWaterSensorsState(int currentstate);
float getPH();
float getO2();
float getEC();


#endif
