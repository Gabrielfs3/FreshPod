#include <Arduino.h>
#include <MQTT.h>
#include <WiFi.h>
#include "WifiEsp32.h"

#include "../lib/ChemicalSensors/SensorEc.h"
#include "../lib/ChemicalSensors/SensorPh.h"
#include "../lib/ChemicalSensors/SensorO2.h"
#include "../lib/WATER/WaterSensors.h"
#include "../lib/WATER/Water_Sensors_SM.h"


WiFiClient net;
MQTTClient client;


enum class trafStatesMachine : int        //estados
{
  STAND_BY,     
  PUBLISH_DATA
};

const int DataMsTelemetry = 1000;             // 1 segundo timeout
int lastSwitchTimeTelemetry = 0;

boolean data_sent;                      //transisitions
boolean timeout_water_sensor;
boolean timeout_temp_hum;
boolean timeout_lum_waterlevel;
boolean MQTT_INIT_NEEDED;


float Pub_ec1,Pub_O2,Pub_pH;
char message_buffer[150];
const char* outTopicwater = "/water/sensor/measurement/PHO2CE";
const char* lastWiFiState = "DISCONNECTED";

void Publish_Water()
{
      Pub_pH = getPH();
      Pub_ec1 = getEC();
      Pub_O2 = getO2();

        sprintf(message_buffer,"{\"pH\":%f,\"o2\":%f,\"ce\":%f}", Pub_pH, Pub_O2, Pub_ec1);
        client.publish(outTopicwater,message_buffer);
  
}
/////////Conectar ao MQTT///////
void MQTT_INIT()
{
  client.begin("public.cloud.shiftr.io", net);                          //broker mqtt
  while (!client.connect("Esp32FreshPod", "public", "public")) {
  Serial.print(".");
   delay(1000);
  }
  Serial.println("\nconnected to mqtt broker!");
  MQTT_INIT_NEEDED=0;
}


trafStatesMachine stateTelemetry = trafStatesMachine::STAND_BY;  //initial state

void runSwitchCaseTelemetry(int timeMsTelemetry,const char* WiFi_state)        //state machine
{


  if(lastWiFiState == "AP_CONNECTED" && WiFi_state == "INTERNET_CONNECTED")
  {
    MQTT_INIT();
  }

  if((timeout_water_sensor==1 || timeout_temp_hum==1 || timeout_lum_waterlevel==1) && (WiFi_state == "INTERNET_CONNECTED"))
  {
    stateTelemetry = trafStatesMachine::PUBLISH_DATA;
  }

  if(data_sent == 1 )
  {
    stateTelemetry = trafStatesMachine::STAND_BY;
  }


  //Serial.println(timeout_water_data);
  
  switch(stateTelemetry)
  {
    case trafStatesMachine::STAND_BY:             
    {
      // temos que fazer if aqui visto que pode ler qualquer um doos 3 tipos de sensores////////////
      if(timeMsTelemetry - lastSwitchTimeTelemetry >= DataMsTelemetry)
      {
        lastSwitchTimeTelemetry = timeMsTelemetry;
        timeout_water_sensor = 1;

        Serial.println("Standy By Telemetry");
        data_sent = 0;
      }
      if(lastWiFiState == "AP_CONNECTED" && WiFi_state == "INTERNET_CONNECTED")
      {
        MQTT_INIT();
      }
      break;
    }
    case trafStatesMachine::PUBLISH_DATA:
    {
      Publish_Water();
      data_sent = 1;       
      timeout_water_sensor = 0;
      //timeout_lum_waterlevel = 0;
      //timeout_temp_hum = 0;
      Serial.println("Publishing Data");
    }
  }

  lastWiFiState = WiFi_state;

}

void MQTT_TASKS(const char* WiFi_state)
{
  int timeMsTelemetry = millis();
  runSwitchCaseTelemetry(timeMsTelemetry,WiFi_state);
}
