#include <Arduino.h>
#include "..\lib\EnvironmentSensors\EnvironmentSensors.h"


//variables & GPIO
#define UPDATE_TIME                     500 // for testing- prints ocorrem 3x a cada sampling da task 

#define environment_sampling            1000 //1sec

//------[DHT11]------
#define yellow_pin  27
#define DHTTYPE 11 //modelo do equipamento 

//------[luminousity]------
#define SCL                             22
#define SCA                             21

//------[geral]------
#define baudrate    115200
//---------------------
float water,temp, humi;

environment ambi(yellow_pin, DHTTYPE, SCL, SCA, baudrate);

void setup() 
{
    ambi.Init();
    Serial.begin(baudrate); // For testing
}

void loop() 
{   
    static unsigned long previous_time=millis();
   
    if (millis()-previous_time > environment_sampling)
    {
        Serial.println("environment tasks");
        ambi.environment_sensor_tasks();
        previous_time=millis();
    }
    static unsigned long last = millis();

    if ((millis() - last) > UPDATE_TIME) 
    {   
        Serial.println("return values");
        last = millis();
        Serial.print(" Luminosity  [LX]    : "); Serial.println(ambi.return_luminousity(),2);
        Serial.print(" Temperature [ºC]    : "); Serial.println(ambi.return_temperature(),1);
        Serial.print(" Humidity    [%]     : "); Serial.println(ambi.return_humidity(),1);
        Serial.println();
    }
}