#include <Arduino.h>
#include "power_meter.h"

#define UPDATE_TIME                     5000

//estados
enum struct trafStatesPower : int        
{
  STAND_BY_POWER_METER,   
  READ_DATA_POWER_METER
};

//Eventos
boolean data_consumption_readed;                   
boolean timeout_consumption_data;

//global variables
int lastSwitchTime = 0, currentstate =0;

//definir clases
power_meter power_SM(CF_PIN,CF1_PIN,SEL_PIN,CURRENT_MODE,relay_PIN);

void InitPowerMeter()
{
  power_SM.Init();                   //initialize water pins
}

//initial state
trafStatesPower state = trafStatesPower::STAND_BY_POWER_METER; 

int runSwitchCase(int timeMs)        //state machine
{

    if(timeout_consumption_data==1)
    {
      state = trafStatesPower::READ_DATA_POWER_METER;
    }

    if(data_consumption_readed==1)
    {
      state = trafStatesPower::STAND_BY_POWER_METER;
    }

    switch (state)
    {
    case trafStatesPower::STAND_BY_POWER_METER: 
      if((timeMs-lastSwitchTime)> UPDATE_TIME)
          {
            lastSwitchTime = timeMs;
            timeout_consumption_data=1;
            data_consumption_readed = 0;
            currentstate =0;
            //Serial.println("ESTADO  0 "); //debug
          }
        break;
    case trafStatesPower::READ_DATA_POWER_METER:  
      power_SM.power_task(UPDATE_TIME);
      
      data_consumption_readed = 1; 
      timeout_consumption_data=0;
      currentstate =1;
      //Serial.println("ESTADO  1 "); //debug
      break;
    }


return currentstate;
}

void PowerMeterTasks()
{
  int timeMs = millis();
  currentstate = runSwitchCase(timeMs);
}

int returnPowerMeterState(int currentstate)
{
  return currentstate;
}

float getConsumption()
{
    return power_SM.return_energy();
}