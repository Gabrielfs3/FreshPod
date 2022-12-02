#include <Arduino.h>
#include "environment_senso.h"

//variables & GPIO
//------[DHT11]------
#define yellow_pin  27
#define DHTTYPE 11 //modelo do equipamento 

//------[HC_SR04]------
#define echoPin 12
#define trigPin 14

//------[geral]------
#define baudrate    115200
//---------------------
float water,temp, humi;

environment ambi(yellow_pin, DHTTYPE, echoPin, trigPin, baudrate);

void setup() 
{
    ambi.Init();
    Serial.begin(baudrate); // For testing
}

void loop() 
{   Serial.println("Wainting task...");
    ambi.environment_sensor_tasks();
    
    delay(500); // for testing- prints ocorrem 3x a cada sampling da task 
    Serial.print("\nWater Level? ");
    water=ambi.return_Watter();
    Serial.print(water,1);
    Serial.println(" cm");

    Serial.print("\ntemperature? ");
    temp= ambi.return_Temp();
    Serial.print(temp,1);
    Serial.println(" ºC");

    Serial.print("\nHumidity? ");
    humi= ambi.return_humi();
    Serial.print(humi,1);
    Serial.println(" %\t");
}