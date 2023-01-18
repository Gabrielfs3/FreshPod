#include <Arduino.h>
#include <ArduinoJson.h>
#include "EnvironmentSensors_SM.h"
#include "PowerMeter_SM.h"
#include "SensorEc.h"
#include "SensorPh.h"
#include "SensorO2.h"
#include "WaterSensors.h"
#include "Water_Sensors_SM.h"
#include "Telemetry_SM.h"
#include "WiFi_Driver.h"
#include "Control_SM.h"
#include <testeeeprom.h>
#include <StreamUtils.h>


int sensor_pin_ph = 33;
int sensor_pin_o2= 32;
int sensor_pin_ec = 35;
int sensor_pin_pin_echo = 12;
int sensor_pin_pin_trig = 14;


const char* rede = "iPhoneFilipe";
const char* password = "hellofilipe!";


WiFi_Driver wifi_esp(rede,password);


void setup()
{

    Serial.begin(115200);
    EEPROM.begin(512);
    String firstTable= "{\"events\":[{\"type\":\"light\",\"start_time\":\"20:00:00\",\"end_time\":\"20:10:00\",\"on_time\":\"00:05:00\",\"off_time\":\"00:05:00\"},{\"type\":\"water\",\"start_time\":\"06:00:00\",\"end_time\":\"18:00:00\",\"on_time\":\"00:05:00\",\"off_time\":\"00:05:00\"}]}";
    storeTableInEEPROM(firstTable);
    wifi_esp.Init();
    MQTT_INIT();
    WATER_SENSORS_INIT();
    InitEnvironmentSensors();
    InitPowerMeter();
    //testeprom();
}   

void loop() 
{
    WaterSensorsTasks();
   EnvironmentTasks();
   PowerMeterTasks();
   ControlTasks();
   wifi_esp.Task();
   const char* WiFi_state = wifi_esp.return_wifi_state ();
    Serial.println(wifi_esp.return_wifi_state ());
    MQTT_TASKS(WiFi_state);
}