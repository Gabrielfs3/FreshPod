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

    Serial.begin(115200);
    Serial.println("test!");
    hc.Init();
}


bool environment::return_Watter()
{
    //int echoPin= 3;//3;
    //int trigPin=5; //5;
    //int baudrate=115200;

    float control_dist=8.00;

    
    float dist = hc.get_dist();
    Serial.print("Distancia: ");
    Serial.print(dist,1);
    Serial.println(" cm");

    if (dist >= control_dist)
    {
        return true ;
    }
    else
    {
        return false ;
    }

}

bool environment::return_Temp()
{
    //variables
    //int baudrate=115200;
    //int yellow_pin=16;
   // int DHTTYPE=11; //modelo do equipamento 
    float control_tem=23.0;
    float control_humi=90.0;
//-----------------

   
    float temp= DHT11.get_Temp();
    float humi=DHT11.get_Humi();
    Serial.print(temp);
    if (isnan(temp)||isnan(humi))
    {
        Serial.println("Failed to read from DHT");
    }
    else
    {
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.println(" ºC");
        Serial.print("Humidity: ");
        Serial.print(humi);
        Serial.print(" %\t");
    }
    delay(100);

    if (temp>=control_tem)
    {
        return false ;
    }
    else
    {
        return true ;

    }
}