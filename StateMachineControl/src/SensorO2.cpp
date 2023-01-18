#include <Arduino.h>
#include "SensorO2.h"


SensorO2::SensorO2(int pin)
{
    pin_o2=pin;
}

void SensorO2::init()
{
    pinMode(pin_o2,OUTPUT);
}

float SensorO2::get_o2()
{
    for(int i=0;i<10;i++)                                   //Get 10 sample value from the sensor for smooth the value
    { 
        buf[i]=analogRead(pin_o2);                          //Saves 10 samples on a buffer got from the sensor reading
        delay(100);
    }
    for(int i=0;i<9;i++)                                    //Sort the analog from small to large
    {
        for(int j=i+1;j<10;j++)                                 //Sweep all values saved on buffer
        {
            if(buf[i]>buf[j])
            {
                o_2=buf[i];
                buf[i]=buf[j];
                buf[j]=o_2;
            }
        }
    }

    avgValue=0;

    for(int i=2;i<8;i++)                                    //Take the average value of 6 center sample
    {                                                       //Get rid of first 2 and last 2 samples
        avgValue+=buf[i];
    }

    avgValue = ((float)avgValue/6);                         //Calculate mean value of 6 samples
    o2_Value=(float)avgValue*3.3/4096;                      //Convert the analog value into millivolt
    o2_Value=6.060606060*o2_Value;                          //Convert the millivolt into pH value
    return o2_Value;
}