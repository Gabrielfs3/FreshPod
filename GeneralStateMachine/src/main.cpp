#include <Arduino.h>
#include "EnvironmentSensors.h"
#include "SensorEc.h"
#include "SensorPh.h"
#include "SensorO2.h"
#include "WaterSensors.h"
#include "Water_Sensors_SM.h"
#include "Telemetry_SM.h"
#include "WiFi_Driver.h"


int sensor_pin_ph = 33;
int sensor_pin_o2= 32;
int sensor_pin_ec = 35;

const char* rede = "Vodafone-614286";
const char* password = "pdz7bZeF3g";

WiFi_Driver wifi_esp(rede,password);

void setup()
{
    Serial.begin(115200);
    wifi_esp.Init();
    WATER_SENSORS_INIT();
    MQTT_INIT();
   
}

void loop() 
{
   
  WaterSensorsTasks();
  wifi_esp.Task();
  const char* WiFi_state = wifi_esp.return_wifi_state ();
  Serial.println(wifi_esp.return_wifi_state ());
  MQTT_TASKS(WiFi_state);
 
}