#ifndef PowerMeter_SM_h
#define PowerMeter_SM_h

#include "PowerMeter.h"

void InitPowerMeter();
int runPowerSwitchCase();
void PowerMeterTasks();
int returnPowerMeterState(int currentstatePower);
float getConsumption();

#endif