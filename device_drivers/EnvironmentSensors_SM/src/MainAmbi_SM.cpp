#include <Arduino.h>
#include "..\lib\EnvironmentSensors\EnvironmentSensors_SM.h"

#define SERIAL_BAUDRATE                 115200

//global variables
unsigned long previousMillis = 0;  //will store last time 
long period = 2000;         // sampling

void setup()
{
  Serial.begin(SERIAL_BAUDRATE); 
  Serial.println("#############[ Testing SM Environment Sensors ]#############"); //DEBUGs
  InitEnvironmentSensors();
}

void loop() 
{
  EnvironmentTasks();

  //----------------[ For testing ]----------------------------------
  //print consumption value after period time
  unsigned long currentMillis = millis(); // store the current time
  if (currentMillis - previousMillis >= period) // check if sampling passed
  { 
    previousMillis = currentMillis;   // save the last time
    Serial.print("Return values after [Sec]:    "); Serial.println(period/1000);
    Serial.print(" Luminosity  [LX]    : "); Serial.println(getLuminousity(),2);
    Serial.print(" Temperature [ºC]    : "); Serial.println(getTemperature(),1);
    Serial.print(" Humidity    [%]     : "); Serial.println(getHumidity(),2);
    Serial.println();
  }
 //------------------------------------------------------------------------

}