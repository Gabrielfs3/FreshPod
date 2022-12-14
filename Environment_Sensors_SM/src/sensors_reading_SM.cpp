#include <Arduino.h>
#include <MQTT.h>
#include <WiFi.h>
#include "Wifi_esp32.h"


#include "../lib/WATER/sensor_ph.h"
#include "../lib/WATER/sensor_ec.h"
#include "../lib/WATER/sensor_o2.h"
#include "../lib/WATER/water_sensors.h"
#include "../lib/WATER/Water_Sensors_SM.h"
//#include environment sensors

////////////net casa filipe///////////////
const char* ssid = "Vodafone-CAD4A5";
const char* pass = "Dje4dUNe8h3Ffxz8";
//////////////////////////////////////////

WiFiClient net;
MQTTClient client;

Wifi_esp32 wifi_esp32(ssid,pass);

enum class trafStatesMachine : int        //estados
{
  STAND_BY,     
  SEND_DATA
};

const int DataMs = 1000;             // 1 segundo timeout
int lastSwitchTime = 0;

boolean data_sent;                      //transisitions
boolean timeout_water_sensor;
boolean timeout_temp_hum;
boolean timeout_lum_waterlevel;

/*
water_sensors water(sensor_pin_ph, sensor_pin_o2, sensor_pin_ec);
//environment_sensors


void InitWaterSensors()
{
  water.init();                   //initialize water pins
}

//void init_environment_sensors
//initialize environment sensors
*/

/////////verificar se comunicação está bem feita///////
void create_communication_MQTT()
{
  wifi_esp32.connect();
  client.begin("public.cloud.shiftr.io", net);                //broker mqtt
  client.publish("/teste", "publish");
  client.subscribe("teste subscribe");
}
//create communication MQTT
//check MQTT

trafStatesMachine state = trafStatesMachine::STAND_BY;  //initial state

int runSwitchCase(int timeMs)        //state machine
{

  if(timeout_water_sensor==1 || timeout_temp_hum==1 || timeout_lum_waterlevel==1)
  {
    state = trafStatesMachine::SEND_DATA;
  }

  if(data_sent == 1)
  {
    state = trafStatesMachine::STAND_BY;
  }
  //Serial.println(timeout_water_data);
  
  switch(state)
  {
    case trafStatesMachine::STAND_BY:             
    {
      // temos que fazer if aqui visto que pode ler qualquer um doos 3 tipos de sensores////////////
      if(timeMs - lastSwitchTime >= DataMs)
      {
        lastSwitchTime = timeMs;
        timeout_water_sensor = 1;
        //timeout_lum_waterlevel = 1;
        //timeout_temp_hum = 1;
        data_sent = 0;
      }
      break;
    }
    case trafStatesMachine::SEND_DATA:
    {
      // temos que fazer if aqui vsito que temos 2 funções para ler e 3 timeouts//////////
      //water.water_sensors_tasks();
      //ambi.environment_sensors_tasks();     
      data_sent = 1;       
      timeout_water_sensor = 0;
      //timeout_lum_waterlevel = 0;
      //timeout_temp_hum = 0;
    }
  }
}

void WaterSensorsTasks()
{
  int timeMs = millis();
  runSwitchCase(timeMs);
}

/*
//void ambi_sensors_tasks()
{
  int timeMs_env = millis();
  runSwitchCase(timeMs);
}
*/
//void lum_waterlevel