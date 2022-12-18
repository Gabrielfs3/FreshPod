#include <Arduino.h>
#include "..\lib\EnvironmentSensors\EnvironmentSensors_SM.h"
#include "..\lib\EnvironmentSensors\EnvironmentSensors.h"

#define baudrate    115200

#define UPDATE_TIME                     1000 //sampling da ENVIRONMENT task

//estados
enum struct trafStatesEnvironment : int        
{
  STAND_BY_ENVIRONMENT_SENSORS,
  READ_DATA_ENVIRONMENT_SENSORS  
};

//Eventos
boolean data_environment_readed;                   
boolean timeout_environment_data;

//global variables
int lastswitchtimeEnvironment = 0, currentstateEnvironment =0;

//GPIOS
//------[DHT11]------
#define yellow_pin  27
#define DHTTYPE 11 //modelo do equipamento 

//------[luminousity]------
#define SCL                             22
#define SCA                             21

//definir clases
environment ambi(yellow_pin, DHTTYPE, SCL, SCA, baudrate);

void InitEnvironmentSensors()
{
  ambi.Init();                 //initialize Environment sensors
}

//initial state
trafStatesEnvironment state_ambi = trafStatesEnvironment::STAND_BY_ENVIRONMENT_SENSORS; 

int runSwitchCase(int timeMs_ambi)        //state machine
{

    if(timeout_environment_data==1)
    {
      state_ambi = trafStatesEnvironment::READ_DATA_ENVIRONMENT_SENSORS;
    }

    if(data_environment_readed==1)
    {
      state_ambi = trafStatesEnvironment::STAND_BY_ENVIRONMENT_SENSORS;
    }

    switch (state_ambi)
    {
    case trafStatesEnvironment::STAND_BY_ENVIRONMENT_SENSORS: 
      if((timeMs_ambi-lastswitchtimeEnvironment)> UPDATE_TIME)
          {
            lastswitchtimeEnvironment = timeMs_ambi;
            timeout_environment_data=1;
            data_environment_readed = 0;
            currentstateEnvironment =0;
            Serial.println("ESTADO  0 "); //debug
          }
        break;
    case trafStatesEnvironment::READ_DATA_ENVIRONMENT_SENSORS:  
        ambi.environment_sensor_tasks();
        data_environment_readed = 1; 
        timeout_environment_data=0;
        currentstateEnvironment =1;
        Serial.println("ESTADO  1 "); //debug
      break;
    }

return currentstateEnvironment;
}

void EnvironmentTasks()
{
  int timeMs_ambi = millis();
  currentstateEnvironment = runSwitchCase(timeMs_ambi);
}

int returnEnvironmentState(int currentstateEnvironment)
{
  return currentstateEnvironment;
}

float getLuminousity()
{
    return ambi.return_luminousity();
}

float getTemperature()
{
    return ambi.return_temperature();
}
float getHumidity()
{
    return ambi.return_humidity();
}


