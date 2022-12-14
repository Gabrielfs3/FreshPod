#include <Arduino.h>

#include "../lib/WATER/sensor_ph.h"
#include "../lib/WATER/sensor_ec.h"
#include "../lib/WATER/sensor_o2.h"
#include "../lib/WATER/water_sensors.h"
#include "../lib/WATER/Water_Sensors_SM.h"


enum class trafStatesWater : int        //estados
{
  stand_By_Water_Sensors,     
  read_Data_Water_Sensors
};

const int WaterDataMs = 1000;             // 1 S
int lastSwitchTime = 0;
int nextSwitchInterval = 0;

boolean data_readed;                      //transisitions
boolean timeout_water_data;

water_sensors water(sensor_pin_ph, sensor_pin_o2, sensor_pin_ec);


void InitWaterSensors()
{
  water.init();                   //initialize water pins
}

trafStatesWater state = trafStatesWater::stand_By_Water_Sensors;  //initial state

void runSwitchCase(int timeMs)        //state machine
{

  if(timeMs-lastSwitchTime>= WaterDataMs)
  {
      timeout_water_data=1;
  }
  else
  {
      timeout_water_data=0;
  }

  switch(state)
  {

    case trafStatesWater::stand_By_Water_Sensors:
    {
      if(timeout_water_data==1)          //timeout_water_data
      {
          state = trafStatesWater::read_Data_Water_Sensors;
          lastSwitchTime = timeMs;
          data_readed = 0;
      }

      break;
    }
    case trafStatesWater::read_Data_Water_Sensors:
    {
          water.water_sensors_tasks();      
          data_readed = 1;            
          state = trafStatesWater::stand_By_Water_Sensors;
    }
  }

}

void WaterSensorsTasks()
{
  int timeMs = millis();
  runSwitchCase(timeMs);
}