#ifndef EnvironmentSensors_SM_h
#define EnvironmentSensors_SM_h

#include "..\lib\EnvironmentSensors\EnvironmentSensors.h"

void InitEnvironmentSensors();
int runSwitchCase();
void EnvironmentTasks();
int returnEnvironmentState(int currentstateEnvironment);
float getLuminousity();
float getTemperature();
float getHumidity();

#endif