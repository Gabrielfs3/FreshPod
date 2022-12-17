#include <Arduino.h>
#include "Power_Meter_SM.h"
#include "power_meter.h"

#define SERIAL_BAUDRATE                 115200
/*
// GPIOs
#define relay_PIN                       18
#define SEL_PIN                         25
#define CF1_PIN                         33
#define CF_PIN                          32
#define CURRENT_MODE                    LOW
*/

int relay_PIN =18,SEL_PIN=25,CF1_PIN=33,CF_PIN =32;
char CURRENT_MODE=LOW;

//global variables
unsigned long previousMillis = 0;  //will store last time 
long period = 60000;         // sampling

void setup()
{
  Serial.begin(SERIAL_BAUDRATE); 
  Serial.println("#############[ Testing SM POWER METER ]#############"); //DEBUGs
  InitPowerMeter();
}

void loop() 
{
  PowerMeterTasks();

  //----------------[ For testing ]----------------------------------
  //print consumption value after period time
  unsigned long currentMillis = millis(); // store the current time
  if (currentMillis - previousMillis >= period) // check if sampling passed
  { 
    previousMillis = currentMillis;   // save the last time
    Serial.print("Power Consumption? [Wh]   :   "); Serial.println(getConsumption(),3);
    Serial.println();
  }
 //------------------------------------------------------------------------

}