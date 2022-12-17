#ifndef Telemetry_SM_h
#define Telemetry_SM_h


void Publish_Water();
void MQTT_INIT();
void runSwitchCaseTelemetry(int timeMs,const char* WiFi_state);   
void MQTT_TASKS(const char* WiFi_state);


#endif
