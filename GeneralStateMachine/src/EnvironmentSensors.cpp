#include <Arduino.h>
#include "..\lib\EnvironmentSensors\DHT11_driver.h"
#include "..\lib\EnvironmentSensors\LuminositySensor.h"
#include "..\lib\EnvironmentSensors\EnvironmentSensors.h"

#include <NTPClient.h>
#include <WiFiUdp.h>

//variavies globais 
static float lux, humi,temp;
int _yellow_pin,_type,_scl_,_sca_;
long int  TimeStamp_lumi, TimeStamp_temp, TimeStamp_humi;

WiFiUDP ntpUDP_Environment;
NTPClient timeClient_Environment(ntpUDP_Environment);

 environment:: environment(int pin, int DHTTYPE, int _scl, int _sca, int baud) //Definir GPIO'S
{   
    //======DHT11======
    yellow_pin=pin;
    type=DHTTYPE;
    
    //======luminosity======
    scl=_scl;
    sca=_sca;
    //=========[passar para publico]=====
    _scl_=_scl;
    _sca_=_sca;
    _yellow_pin=pin;
    _type=DHTTYPE;

    baudrate=baud;
}

//-----------[definir classes]-------------
LuminositySensor luxs(_scl_, _sca_);
DHT11class DHT11(_yellow_pin,_type);
//------------------------------------------

void environment::Init()
{
    Serial.begin(baudrate);
    luxs.Init();
    DHT11.Init();
}

void environment::environment_sensor_tasks() //Executa a task de sampling em sampling
{
    lux=luxs.get_luminosity();
    TimeStamp_lumi = timeClient_Environment.getEpochTime();
    temp= DHT11.get_temperature();
    TimeStamp_temp = timeClient_Environment.getEpochTime();
    humi=DHT11.get_humidity();
    TimeStamp_humi = timeClient_Environment.getEpochTime();
}

float environment::return_luminousity()
{
    return lux;
}

float environment::return_temperature()
{
    if (isnan(temp))
    {
        //Serial.println("Failed to read from DHT");
        return 0;
    }
    else
    {
        return temp;
    }
}

float environment::return_humidity()
{
    if (isnan(humi))
    {
        //Serial.println("Failed to read from DHT");
        return 0;
    }
    else
    {
        return humi;
    }
}

// return TS's
long int environment::return_TS_lumi()
{
    return TimeStamp_lumi;
}

long int environment::return_TS_temp()
{
   return TimeStamp_temp;
}
long int environment::return_TS_humi()
{
    return TimeStamp_humi;
}