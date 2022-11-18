#include <Arduino.h>
#include <C:\Users\miigu\OneDrive - Universidade de Aveiro\Makeit_PI\Code\environment_sensors\include\environment_senso.h>


//variables
//------[DHT11]------
int yellow_pin=16;
int DHTTYPE=11; //modelo do equipamento 

//------[HC_SR04]------
int echoPin=3;
int trigPin=5;

//------[geral]------
int baudrate=115200;
//---------------------
float water;
float temp;
float humi;

environment ambi(yellow_pin, DHTTYPE, echoPin, trigPin, baudrate);

void setup() 
{
    ambi.Init();
    Serial.begin(baudrate); // For testing
}

void loop() 
{   
    delay(1000);
    Serial.print("\nWater Level? ");
    water=ambi.return_Watter();
    Serial.print(water,1);
    Serial.println(" cm");

    delay(1000);
    Serial.print("\ntemperature? ");
    temp= ambi.return_Temp();
    Serial.print(temp,1);
    Serial.println(" ºC");
    
    delay(1000);
    Serial.print("\nHumidity? ");
    humi= ambi.return_humi();
    Serial.print(humi,1);
    Serial.println(" %\t");
    
}