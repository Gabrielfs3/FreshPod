#include <Arduino.h>

#include "../lib/WATER/sensor_ph.h"
#include "../lib/WATER/sensor_ec.h"
#include "../lib/WATER/sensor_o2.h"
#include "../lib/WATER/water_sensors.h"
#include "../lib/WATER/Water_Sensors_SM.h"
#include "../lib/TELEMETRY/Telemetry_SM.h"
#include "../lib/TELEMETRY/WiFi_Driver.h"

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