#include <Arduino.h>
#include "sensor_ec.h"


sensorEC::sensorEC(int pin)
{
    pin_ec=pin;
}


void sensorEC::init()
{
    pinMode(pin_ec,INPUT);
}


float sensorEC::get_EC()
{
   for(int i=0;i<10;i++)                               //Get 10 sample value from the sensor for smooth the value
    { 
        buf[i]=analogRead(pin_ec);                   //Saves 10 samples on a buffer got from the sensor reading
        delay(100);
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
    avgValue = ((float)avgValue/6);                 //Calculate mean value of 6 samples
    ec_Value=(float)avgValue*3.3/4096;              //Convert the analog value into millivolt
    ec_Value=6.060606060*ec_Value;                  //Convert the millivolt into EC value
    return ec_Value;
}