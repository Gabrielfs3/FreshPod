#ifndef Telemetry_SM2_h
#define Telemetry_SM2_h
#include <stdint.h>
#include <Arduino.h>
#include <ArduinoJson.h>

void Publish_Water();
void Publish_environment();
void Publish_Consumption();
void MQTT_INIT();
void runSwitchCaseTelemetry(int timeMs,const char* WiFi_state);   
void MQTT_TASKS(const char* WiFi_state);
void susbcribeControl();
void callback1(char* topic, byte* payload, unsigned int length);
String Controlloop();
String returnnewtable();


#endif