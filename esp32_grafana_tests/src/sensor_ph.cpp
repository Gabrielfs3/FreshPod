#include <Arduino.h>
#include "sensor_ph.h"



sensorpH::sensorpH(int pin)
{
    pin_ph=pin;
}

void sensorpH::init()
{
    pinMode(pin_ph,INPUT);
}

float sensorpH::get_pH()
{
    for(int i=0;i<10;i++)                           //Get 10 sample value from the sensor for smooth the value
    { 
        buf[i]=analogRead(pin_ph);               //Saves 10 samples on a buffer got from the sensor reading
        delay(100);
    }

    for(int i=0;i<9;i++)                            //Sort the analog from small to large
    {
        for(int j=i+1;j<10;j++)                         //Sweep all values saved on buffer
        {
            if(buf[i]>buf[j])
            {
                ph=buf[i];
                buf[i]=buf[j];
                buf[j]=ph;
            }
        }
    }

    avgValue=0;

    for(int i=2;i<8;i++)                            //Take the average value of 6 center sample
    {                                               //Get rid of first 2 and last 2 samples
        avgValue+=buf[i];
    }

    avgValue = ((int)avgValue/6);                 //Calculate mean value of 6 samples*/
    ph_Value=analogRead(pin_ph)*3.3/4096;         //Convert the analog value into millivolt
    ph_Value=4.24242424*ph_Value;                 //Convert the millivolt into pH value
    return ph_Value;
}