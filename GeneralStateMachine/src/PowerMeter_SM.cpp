#include <Arduino.h>
#include "PowerMeter_SM.h"

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
int lastswitchtimePower = 0, currentstatePower =0;

int relay_PIN =18,SEL_PIN=25,CF1_PIN=33,CF_PIN =32;
char CURRENT_MODE=LOW;

//definir clases
PowerMeter power_SM(CF_PIN,CF1_PIN,SEL_PIN,CURRENT_MODE,relay_PIN);

void InitPowerMeter()
{
  power_SM.Init();                   //initialize water pins
}

//initial state
trafStatesPower state = trafStatesPower::STAND_BY_POWER_METER; 

int runPowerSwitchCase(int timeMs)        //state machine
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
      if((timeMs-lastswitchtimePower)> UPDATE_TIME)
          {
            lastswitchtimePower = timeMs;
            timeout_consumption_data=1;
            data_consumption_readed = 0;
            currentstatePower =0;
            //Serial.println("ESTADO  0 "); //debug
          }
        break;
    case trafStatesPower::READ_DATA_POWER_METER:  
      power_SM.power_task(UPDATE_TIME);
      
      data_consumption_readed = 1; 
      timeout_consumption_data=0;
      currentstatePower =1;
      //Serial.println("ESTADO  1 "); //debug
      break;
    }


return currentstatePower;
}

void PowerMeterTasks()
{
  int timeMs = millis();
  currentstatePower = runPowerSwitchCase(timeMs);
}

int returnPowerMeterState(int currentstatePower)
{
  return currentstatePower;
}

float getConsumption()
{
    return power_SM.return_energy();
}