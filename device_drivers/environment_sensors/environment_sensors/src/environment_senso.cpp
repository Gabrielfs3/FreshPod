#include <Arduino.h>

#include <C:\Users\miigu\OneDrive - Universidade de Aveiro\Makeit_PI\Code\environment_sensors\include\DHT11_driver.h>
#include <C:\Users\miigu\OneDrive - Universidade de Aveiro\Makeit_PI\Code\environment_sensors\include\hc_sr04.h>
#include <C:\Users\miigu\OneDrive - Universidade de Aveiro\Makeit_PI\Code\environment_sensors\include\environment_senso.h>
 
 int echo;
 int trig;
 int tipos;
 int pinos;

 environment:: environment(int pin, int DHTTYPE, int pinx, int piny, int baud)
{
    yellow_pin=pin;
    type=DHTTYPE;

    echoPin=pinx;
    trigPin=piny;
    baudrate=baud;

    echo=echoPin;
    trig=trigPin;
    pinos=yellow_pin;
    tipos=type;
}

//definir class
hc_sr04 hc(echo,trig);
DHT11class DHT11(pinos,tipos);


void environment::Init()
{

    Serial.begin(baudrate);
    hc.Init();
}


float environment::return_Watter()
{
    float control_dist=8.00;

    float dist = hc.get_dist();
    return dist;
}

float environment::return_Temp()
{

    float temp= DHT11.get_Temp();
    if (isnan(temp))
    {
        Serial.println("Failed to read from DHT");
    }
    else
    {
        return temp;

    }
}

float environment::return_humi()
{

    float humi=DHT11.get_Humi();
    if (isnan(humi))
    {
        Serial.println("Failed to read from DHT");
    }
    else
    {
        return humi;
    }
}