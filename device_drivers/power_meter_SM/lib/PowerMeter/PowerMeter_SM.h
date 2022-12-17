#ifndef PowerMeter_SM_h
#define PowerMeter_SM_h

#include "PowerMeter.h"

void InitPowerMeter();
int runSwitchCase();
void PowerMeterTasks();
int returnPowerMeterState(int currentstate);
float getConsumption();

#endif