#include <Arduino.h>
#include <MQTT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "WifiEsp32.h"

#include "../lib/ChemicalSensors/SensorEc.h"
#include "../lib/ChemicalSensors/SensorPh.h"
#include "../lib/ChemicalSensors/SensorO2.h"
#include "../lib/WATER/HC_SR04_driver.h"
#include "../lib/WATER/WaterSensors.h"
#include "../lib/WATER/Water_Sensors_SM.h"

#include "EnvironmentSensors_SM.h"
#include "PowerMeter_SM.h"


WiFiClient net;
PubSubClient client(net);
//MQTTClient client;


enum class trafStatesMachine : int        //estados
{
  STAND_BY,     
  PUBLISH_DATA
};

//-----------[ Period ]-----------------
const int DataMsTelemetry = 1000;             //Apagar

const int DataMsTelemetry_Water = (1000);          // 1 sec timeout ---alterar
const int DataMsTelemetry_Environment = 10000;        // 10 segundos timeout---aletrar para 30
const int DataMsTelemetry_Power = 60000;              // 1 min timeout


boolean data_sent;                      //transisitions
boolean timeout_water_sensor;
boolean timeout_Environment;
boolean timeout_Power_Meter;
boolean MQTT_INIT_NEEDED;

//---------[ Water]-----------
float Pub_ec1,Pub_O2,Pub_pH, Pub_WL;
long int Pub_TS_ph, Pub_TS_o2, Pub_TS_ec, Pub_TS_WL;
const char* outTopicwater = "/water/sensor/measurement/PHO2CEWL";
char message_water_buffer[150];
int lastSwitchTimeWater  = 0;


//---------[ Power Meter]-----------
float Pub_Consumption;
long int Pub_TS_Consumption;
const char* outTopicPower ="/Power/sensor/measurement/Consumo";
char message_Power_buffer[150];
int lastSwitchTimePower  = 0;

//---------[ Environment]-----------
float Pub_Luminousity, Pub_Temperature, Pub_Humidity;
long int Pub_TS_Luminousity, Pub_TS_Temperature, Pub_TS_Humidity;
const char* outTopicEnvironment ="/Environment/sensor/measurement/LumiTempHumi";
char message_Environment_buffer[150];
int lastSwitchTimeEnvironment  = 0;

const char* broker = "192.168.1.122";
const char* lastWiFiState = "DISCONNECTED";

void Publish_Water()
{
      Pub_pH = getPH();
      Pub_TS_ph = get_TS_ph();
      Pub_ec1 = getEC();
      Pub_TS_ec = get_TS_ec();
      Pub_O2 = getO2();
      Pub_TS_o2 = get_TS_o2();
      Pub_WL = getWL();
      Pub_TS_WL = get_TS_water_level();

        //sprintf(message_buffer,"{\"pH\":%f,\"o2\":%f,\"ce\":%f}", Pub_pH, Pub_O2, Pub_ec1);
        sprintf(message_water_buffer,"{\"pH\":{\"timestamp\":%ld,\"value\":%f},\"o2\":{\"timestamp\":%ld,\"value\":%f},\"ec\":{\"timestamp\":%ld,\"value\":%f},\"WL\":{\"timestamp\":%ld,\"value\":%f}}", Pub_TS_ph, Pub_pH, Pub_TS_o2 , Pub_O2, Pub_TS_ec, Pub_ec1, Pub_WL, Pub_TS_WL);
        client.publish(outTopicwater,message_water_buffer);
}

void Publish_environment()
{
  Pub_Luminousity=getLuminousity();
  Pub_TS_Luminousity=get_ts_luminousity(); 
  Pub_Temperature=getTemperature();
  Pub_TS_Temperature=get_ts_temperature();
  Pub_Humidity=getHumidity();
  Pub_TS_Humidity=get_ts_humidity();  

  sprintf(message_Environment_buffer,"{\"Lumi\":{\"timestamp\":%ld,\"value\":%f},\"Temp\":{\"timestamp\":%ld,\"value\":%f},\"Humi\":{\"timestamp\":%ld,\"value\":%f}}",Pub_TS_Luminousity,Pub_Luminousity, Pub_TS_Temperature,Pub_Temperature,Pub_TS_Humidity,Pub_Humidity);
  client.publish(outTopicEnvironment,message_Environment_buffer);
}

void Publish_Consumption()
{
  Pub_Consumption=getConsumption();
  Pub_TS_Consumption=get_ts_consumption();

  sprintf(message_Environment_buffer,"{\"timestamp\":%ld,\"value\":%f}",Pub_TS_Consumption,Pub_Consumption);

  //sprintf(message_Environment_buffer,"{\"Consumo\":{\"timestamp\":%ld,\"value\":%f}", Pub_TS_Consumption,Pub_Consumption);
  client.publish(outTopicPower,message_Environment_buffer);
}


/////////Conectar ao MQTT///////
void MQTT_INIT()
{
  //client.begin(broker, net);                          //broker mqtt
  client.setServer(broker,1883);
  //while (!client.connect("Esp32FreshPod", "public", "public")) {
  //Serial.print(".");
  // delay(1000);
  //}
   while(!client.connected()) 
 {
  Serial.print("\nConnected to");
  Serial.println(broker);
  if(client.connect("400"))
  {
    Serial.print("\nConnected to");
    Serial.println(broker);
  }else
  {
    Serial.println("\nTrying connect again");
    delay(5000);
  }
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

  if((timeout_water_sensor==1 || timeout_Environment==1 || timeout_Power_Meter==1) && (WiFi_state == "INTERNET_CONNECTED"))
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

      //#################[ Telemetry_Water] ############################
      if(timeMsTelemetry - lastSwitchTimeWater >= DataMsTelemetry_Water)
      {
        lastSwitchTimeWater = timeMsTelemetry;
        timeout_water_sensor = 1;

        Serial.println("Standy By  Water Telemetry");
        data_sent = 0;
      }
    
      //#################[ Telemetry_Environment] ############################
      if(timeMsTelemetry - lastSwitchTimeEnvironment >= DataMsTelemetry_Environment)
        {
          lastSwitchTimeEnvironment = timeMsTelemetry;
          timeout_Environment = 1;

          Serial.println("Standy By Environment Telemetry");
          data_sent = 0;
        }    

      //#################[ Telemetry_Power_Meter] ############################
      if(timeMsTelemetry - lastSwitchTimePower >= DataMsTelemetry_Power)
        {
          lastSwitchTimePower = timeMsTelemetry;
          timeout_Power_Meter = 1;

          Serial.println("Standy By  Power Meter Telemetry");
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
      //deve de haver formas mais eficientes de implemnetar isto
       if(timeout_water_sensor==1) 
      {
        Publish_Water();
        timeout_water_sensor=0;
      }
      if(timeout_Environment==1 )
      {
        Publish_environment();
        timeout_Environment=0;
      }
      if(timeout_Power_Meter==1)
      {
        Publish_Consumption();
        timeout_Power_Meter=0;
      }
      
      data_sent = 1;       
      //timeout_water_sensor = 0;
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
