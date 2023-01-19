#include <Arduino.h>

#include "../lib/WATER/WaterSensors.h"
#include "../lib/WATER/Water_Sensors_SM.h"


enum class trafStatesWater : int        //estados
{
  STAND_BY_WATER_SENSORS,     
  READ_DATA_WATER_SENSORS
};

const int WaterDataMs = 1000;             // 1 segundo timeout
int lastSwitchTime = 0;

boolean data_readed;                      //transisitions
boolean timeout_water_data;

WaterSensors water(sensor_pin_pin_echo, sensor_pin_pin_trig);

int currentstate =0;

float _o2, _ph, _ec, _water_level, _temperature;
long int TS_ph, TS_o2, TS_ec, TS_water_level, TS_temperature;

void WATER_SENSORS_INIT()
{
  water.init();                   //initialize water pins
  _water_level = 0;
}

trafStatesWater stateWater = trafStatesWater::STAND_BY_WATER_SENSORS;  //initial state

int runSwitchCaseWater(int timeMs)        //state machine
{

  if(timeout_water_data==1)
  {
    stateWater = trafStatesWater::READ_DATA_WATER_SENSORS;
  }
  if(data_readed == 1)
  {
    stateWater = trafStatesWater::STAND_BY_WATER_SENSORS;
  }
  //Serial.println(timeout_water_data);
  
  switch(stateWater)
  {
    case trafStatesWater::STAND_BY_WATER_SENSORS:             
    {
      if(timeMs-lastSwitchTime>= WaterDataMs)
        {
          lastSwitchTime = timeMs;
          timeout_water_data=1;
          data_readed = 0;
          currentstate =0;
          Serial.println("Stand By Water");
        }
      break;
    }
    case trafStatesWater::READ_DATA_WATER_SENSORS:
    {
      water.water_sensors_tasks();   
      _ph = water.return_ph();
      TS_ph = water.return_TS_ph();
      _o2 = water.return_o2();
      TS_o2 = water.return_TS_o2(); 
      _ec = water.return_ec();
      TS_ec = water.return_TS_ec();
      _water_level = water.return_water_level();
      TS_water_level = water.return_TS_water_level();
	    _temperature = water.return_temp();
          TS_temperature = water.return_TS_temp();          
          /*ph++;
          o2++;
          ec++;
          if(ph == 10)
          {
            ph =0;
            o2=0;
            ec=0;
          }*/
          Serial.println("Reading Data Water");
          data_readed = 1;         
          timeout_water_data=0;
          currentstate=1;
    }
  }

  return currentstate;
}

void WaterSensorsTasks()
{
  int timeMs = millis();
  currentstate = runSwitchCaseWater(timeMs);
}

int returnWaterSensorsState(int currentstate)
{
  return currentstate;
}


float getPH()
{
  return _ph;
}

float getEC()
{
  return _ec;
}

float getO2()
{
  return _o2;
}

float getWL()
{
  return _water_level;
}

float gettemperature()
{
  return _temperature;
}

long int get_TS_ph()
{
  return TS_ph;
}

long int get_TS_o2()
{
  return TS_o2;
}

long int get_TS_ec()
{
  return TS_ec;
}

long int get_TS_water_level()
{
  return TS_water_level;
}

float get_TS_temperature()
{
  return TS_temperature;
}