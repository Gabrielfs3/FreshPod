#include <Arduino.h>
#include "EnvironmentSensors_SM.h"
#include "PowerMeter_SM.h"
#include "SensorEc.h"
#include "SensorPh.h"
#include "SensorO2.h"
#include "WaterSensors.h"
#include "Water_Sensors_SM.h"
#include "Telemetry_SM.h"
#include "WiFi_Driver.h"
#include "GPIOs.h"


const char* rede = "Vodafone-624E88";
const char* password = "xGJXZcg7Jd";


WiFi_Driver wifi_esp(rede,password);


void setup()
{
  Serial.begin(115200);
  wifi_esp.Init();
  WATER_SENSORS_INIT();
  InitEnvironmentSensors();
  InitPowerMeter();
  MQTT_INIT();
}

void loop() 
{
  WaterSensorsTasks();
  EnvironmentTasks();
  PowerMeterTasks();
  wifi_esp.Task();
  const char* WiFi_state = wifi_esp.return_wifi_state ();
  Serial.println(wifi_esp.return_wifi_state ());
  MQTT_TASKS(WiFi_state);
}