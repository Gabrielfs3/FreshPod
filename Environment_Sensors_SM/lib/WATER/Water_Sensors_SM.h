#ifndef Water_Sensors_SM_h
#define Water_Sensors_SM_h

#include "../lib/WATER/water_sensors.h"

void InitWaterSensors();
int runSwitchCase(int timeMs);
void WaterSensorsTasks();
int returnWaterSensorsState(int currentstate);
float getPH();
float getO2();
float getEC();


#endif
