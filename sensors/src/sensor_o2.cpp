#include <Arduino.h>
#include "sensor_o2.h"


sensorO2::sensorO2(int pin)
{
    pin_o2=pin;
}

void sensorO2::init()
{
    pinMode(pin_o2,OUTPUT);
}

void sensorO2::volt_O2()
{
    /*for(int i=0;i<10;i++)                                   //Get 10 sample value from the sensor for smooth the value
    { 
        buf[i]=analogRead(SensorPin);                       //Saves 10 samples on a buffer got from the sensor reading
        delay(1000);
    }
    for(int i=0;i<9;i++)                                    //Sort the analog from small to large
    {
    for(int j=i+1;j<10;j++)                                 //Sweep all values saved on buffer
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

    for(int i=2;i<8;i++)                                    //Take the average value of 6 center sample
    {                                                       //Get rid of first 2 and last 2 samples
        avgValue+=buf[i];
    }
  
    //Serial.print("    niveis:");

    avgValue = ((float)avgValue/6);                         //Calculate mean value of 6 samples
    //Serial.print(avgValue,2);                               //Print value with 2 decimal digits*/
    o2_Value=(float)avgValue*3.3/4096*6.060606060;                 //Convert the analog value into millivolt
    //Serial.print("    vol:");  

    //Serial.print(o2_Value,2);                                //Print value with 2 decimal digits
    //o2_Value=6.060606060*o2_Value;                            //Convert the millivolt into pH value
}

float sensorO2::get_O2()
{
    return o2_Value;
}