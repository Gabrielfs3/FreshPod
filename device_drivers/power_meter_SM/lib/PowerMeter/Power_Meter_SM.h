#ifndef Power_Meter_SM_h
#define Power_Meter_SM_h

#include "power_meter.h"

void InitPowerMeter();
int runSwitchCase();
void PowerMeterTasks();
int returnPowerMeterState(int currentstate);
float getConsumption();

#endif