#include <Arduino.h>


#include "../lib/WATER/water_sensors.h"
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

water_sensors water(sensor_pin_ph, sensor_pin_o2, sensor_pin_ec);

int currentstate =0;

float o2, ph, ec;

void InitWaterSensors()
{
  water.init();                   //initialize water pins
}

trafStatesWater state = trafStatesWater::STAND_BY_WATER_SENSORS;  //initial state

int runSwitchCase(int timeMs)        //state machine
{

  if(timeout_water_data==1)
  {
    state = trafStatesWater::READ_DATA_WATER_SENSORS;
  }
  if(data_readed == 1)
  {
    state = trafStatesWater::STAND_BY_WATER_SENSORS;
  }
  //Serial.println(timeout_water_data);
  
  switch(state)
  {
    case trafStatesWater::STAND_BY_WATER_SENSORS:             
    {
      if(timeMs-lastSwitchTime>= WaterDataMs)
        {
          lastSwitchTime = timeMs;
          timeout_water_data=1;
          data_readed = 0;
          currentstate =0;
        }
      break;
    }
    case trafStatesWater::READ_DATA_WATER_SENSORS:
    {
          water.water_sensors_tasks();   
          ph = water.return_pH();  
          o2 = water.return_O2(); 
          ec = water.return_EC();
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
  currentstate = runSwitchCase(timeMs);
}

int returnWaterSensorsState(int currentstate)
{
  return currentstate;
}


float getPH()
{
  return ph;
}

float getEC()
{
  return ph;
}

float getO2()
{
  return o2;
}
