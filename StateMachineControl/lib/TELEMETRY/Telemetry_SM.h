#ifndef Telemetry_SM_h
#define Telemetry_SM_h


void Publish_Water();
void Publish_environment();
void Publish_Consumption();
void MQTT_INIT();
void runSwitchCaseTelemetry(int timeMs,const char* WiFi_state);   
void MQTT_TASKS(const char* WiFi_state);


#endif
