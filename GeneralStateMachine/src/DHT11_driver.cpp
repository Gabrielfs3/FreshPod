#include "DHT.h"
#include <Arduino.h>
#include "DHT11_driver.h"

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

float DHT11class::get_Temp()
{
    return dht.readTemperature();
}

float DHT11class::get_Humi()
{
    return dht.readHumidity();
}