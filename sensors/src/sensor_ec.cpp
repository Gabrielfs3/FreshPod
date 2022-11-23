#include <Arduino.h>
#include "sensor_ec.h"


sensorEC::sensorEC(int pin)
{
    pin_ec=pin;
}


void sensorEC::init()
{
    pinMode(pin_ec,OUTPUT);
}

void sensorEC::volt_EC()
{
   /* for(int i=0;i<10;i++)                               //Get 10 sample value from the sensor for smooth the value
    { 
        buf[i]=analogRead(SensorPin);                   //Saves 10 samples on a buffer got from the sensor reading
        delay(1000);
    }
    for(int i=0;i<9;i++)                                //Sort the analog from small to large
    {
    for(int j=i+1;j<10;j++)                             //Sweep all values saved on buffer
    {
        if(buf[i]>buf[j])
        {
            temp=buf[i];
            buf[i]=buf[j];
            buf[j]=temp;
        }
    }
    }

    avgValue=0;

    for(int i=2;i<8;i++)                            //Take the average value of 6 center sample
    {                                               //Get rid of first 2 and last 2 samples
        avgValue+=buf[i];
    }
  
    //Serial.print("    niveis:");

    avgValue = ((float)avgValue/6);                 //Calculate mean value of 6 samples
    //Serial.print(avgValue,2);                       //Print value with 2 decimal digits*/
    ec_Value=(float)avgValue*3.3/4096*4.24242424;        //Convert the analog value into millivolt
    //Serial.print("    vol:");  

    //Serial.print(ec_Value,2);                       //Print value with 2 decimal digits
    //ec_Value=4.24242424*ec_Value;                   //Convert the millivolt into EC value
}

float sensorEC::get_EC()
{
    return ec_Value;
}