#ifndef Water_Sensors_SM_h
#define Water_Sensors_SM_h

#include "../lib/WATER/WaterSensors.h"

void WATER_SENSORS_INIT();
int runSwitchCaseWater(int timeMs);
void WaterSensorsTasks();
int returnWaterSensorsState(int currentstate);
float getPH();
float getO2();
float getEC();
long int get_TS_ph();
long int get_TS_o2();
long int get_TS_ec();

#endif
