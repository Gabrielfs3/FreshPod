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
bool water;
bool temp;

environment ambi(yellow_pin, DHTTYPE, echoPin, trigPin, baudrate);

void setup() 
{
    ambi.Init();
    Serial.begin(baudrate); // For testing
}

void loop() 
{   
    Serial.print("\nWater? ");
    water=ambi.return_Watter();
    Serial.print(water);
    delay(1000);
    Serial.print("\ntemp?\n");
    temp= ambi.return_Temp();
    Serial.print(temp);
    delay(1000);
}