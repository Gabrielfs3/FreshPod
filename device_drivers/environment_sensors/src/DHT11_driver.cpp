#include "C:\Users\miigu\OneDrive - Universidade de Aveiro\Makeit_PI\Code\environment_sensors\include\DHT11_driver.h"
#include "DHT.h"
#include <Arduino.h>

int pino;
int tipo;

DHT11class::DHT11class(int pin, int model)
{ 
    yellow_pin=pin;
    pino=yellow_pin;
    type=model;
    tipo=type;
}

DHT dht(pino ,tipo) ;

void DHT11class::Init()
{
    //debug
    Serial.begin(115200);
    Serial.println("DHT11 test!");
    //
    dht.begin();
}

float DHT11class::get_Temp()
{
    float t = dht.readTemperature();
    return t;
}

float DHT11class::get_Humi()
{
     float h = dht.readHumidity();
     return h;
}