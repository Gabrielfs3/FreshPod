#include <Arduino.h>

#include "../lib/WATER/sensor_ph.h"
#include "../lib/WATER/sensor_ec.h"
#include "../lib/WATER/sensor_o2.h"
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


void InitWaterSensors()
{
  water.init();                   //initialize water pins
}

trafStatesWater state = trafStatesWater::STAND_BY_WATER_SENSORS;  //initial state

void runSwitchCase(int timeMs)        //state machine
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
        }
      break;
    }
    case trafStatesWater::READ_DATA_WATER_SENSORS:
    {
          water.water_sensors_tasks();      
          data_readed = 1;         
          timeout_water_data=0;
    }
  }
}

void WaterSensorsTasks()
{
  int timeMs = millis();
  runSwitchCase(timeMs);
}