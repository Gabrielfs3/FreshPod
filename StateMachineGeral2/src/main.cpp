#include <Arduino.h>
#include <ArduinoJson.h>
//#include "EnvironmentSensors_SM.h"
#include "PowerMeter_SM.h"
#include "SensorEc.h"
#include "SensorPh.h"
#include "SensorO2.h"
#include "WaterSensors.h"
#include "Water_Sensors_SM.h"
#include "Telemetry_SM2.h"
#include "WiFi_Driver.h"
#include "Control_SM.h"
#include <StreamUtils.h>
#include <Actuators.h>
#include <REAL_TIME_CONTROL.h>
#include <Alarmistic_SM.h>
#include "GPIOs_ValTheo.h"

//qkgi0575


const char* rede = "Mala";
const char* password = "qkgi0575";


WiFi_Driver wifi_esp(rede,password);
Real_Time   real_time;

void setup()
{

    Serial.begin(115200);
    EEPROM.begin(2048);
    wifi_esp.Init();
    MQTT_INIT();
    MQTT_INIT_ALARMISTIC();
    Init_AllActuators();
    WATER_SENSORS_INIT();
  //  InitEnvironmentSensors();
    InitPowerMeter();
    susbcribeControl();
    //real_time.Init();
    InitAlarmistic();

}   

void loop() 
{
    WaterSensorsTasks();
    //EnvironmentTasks();
    PowerMeterTasks();
    ControlTasks();
    wifi_esp.Task();
    newTableReceived();
    const char* WiFi_state = wifi_esp.return_wifi_state ();
    MQTT_TASKS(WiFi_state);
    // real_time.Task();
    AlarmisticTasks();
}