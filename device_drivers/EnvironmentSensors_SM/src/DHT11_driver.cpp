#include <Arduino.h>

#include "..\lib\EnvironmentSensors\Grove_Sensor\DHT.h"
#include "..\lib\EnvironmentSensors\DHT11_driver.h"

//Variaveis globais
int pino, tipo;

DHT11class::DHT11class(int pin, int model)
{ 
    yellow_pin=pin;
    type=model;
    //===============
    pino=yellow_pin;
    tipo=type;
}

DHT dht(pino,tipo) ;

void DHT11class::Init()
{
    dht.begin();
}

float DHT11class::get_temperature()
{
    return dht.readTemperature();
}

float DHT11class::get_humidity()
{
    return dht.readHumidity();
}