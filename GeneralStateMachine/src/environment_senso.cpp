#include <Arduino.h>
#include "hc_sr04.h"
#include "DHT11_driver.h"
#include "environment_senso.h"

//sampling
#define environment_sampling 1000 //1sec

//variavies globais 
static float dist, humi,temp;
int _yellow_pin,_type,echo_Pin,trig_Pin;

 environment:: environment(int pin, int DHTTYPE, int pinx, int piny, int baud) //Definir GPIO'S
{   
    //======DHT11======
    yellow_pin=pin;
    type=DHTTYPE;
    //==================
    baudrate=baud;
    //======hc_sr04======
    echoPin=pinx;
    trigPin=piny;
    //==================
    echo_Pin=pinx;
    trig_Pin=piny;
    _yellow_pin=pin;
    _type=DHTTYPE;
}

//-----------[definir classes]-------------
hc_sr04 hc(echo_Pin,trig_Pin);
DHT11class DHT11(_yellow_pin,_type);
//------------------------------------------

void environment::Init()
{
    Serial.begin(baudrate);
    hc.Init();
    DHT11.Init();
}

void environment::environment_sensor_tasks() //Executa a task de sampling em sampling
{
    static unsigned long previous_time=millis();
   
    if (millis()-previous_time > environment_sampling)
    {   
        //.....tasks.............
        dist = hc.get_dist();
        temp= DHT11.get_Temp();
        humi=DHT11.get_Humi();
        //.......................
        previous_time=millis();
    }
}

float environment::return_Watter()
{
    return dist;
}

float environment::return_Temp()
{
    if (isnan(temp))
    {
        Serial.println("Failed to read from DHT");
        return 0;
    }
    else
    {
        return temp;
    }
}

float environment::return_humi()
{
    if (isnan(humi))
    {
        Serial.println("Failed to read from DHT");
        return 0;
    }
    else
    {
        return humi;
    }
}