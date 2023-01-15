#include <Arduino.h>
#include "Alarmistic_SM.h"
#include "GPIOs_ValTheo.h"

void setup() 
{
  Serial.begin(115200);
  InitAlarmistic();
}

void loop() 
{
  AlarmisticTasks();
}