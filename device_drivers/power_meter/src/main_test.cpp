#include <Arduino.h>
#include "power_meter.h"

#define SERIAL_BAUDRATE                 115200

// GPIOs
#define RELAY_PIN                       18
#define SEL_PIN                         25
#define CF1_PIN                         33
#define CF_PIN                          32

//other GPIO 
//#define LED                            21 //SDA
//#define button                         21 //FLASH

#define CURRENT_MODE                    LOW
#define use_interrupts                  true
//#define use_interrupts                  false
#define _pulse_timeout                  500000


//global variables
double current, voltage, power, *p;
unsigned long previousMillis = 0;  //will store last time 
const long period = 6000;         // sampling


//define classes
power_meter pm(CF_PIN,CF1_PIN,SEL_PIN,CURRENT_MODE,use_interrupts,_pulse_timeout);

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE); // For testing
  Serial.println("TESTE MAIN POWER");
  pm.Init();
}

void loop() 
{   
  pm.power_task();

  unsigned long currentMillis = millis(); // store the current time
  if (currentMillis - previousMillis >= period) // check if sampling passed
  { 
    previousMillis = currentMillis;   // save the last time
    Serial.println("Power Consumption? [kWh]"); //Serial.print(*(p+5));//Serial.println(power_meter.get_energy());
    p=pm.return_energy();
    int i=0;
    while(*(p+i)!='\0')  
    {
      Serial.println(*(p+i));
      i++;
    }
    Serial.println(); 
  } 
}