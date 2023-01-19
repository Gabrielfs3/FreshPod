#include <Arduino.h>
#include <DS18B20.h>
#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <DFRobot_ADS1115.h>


//#define PH_PIN 33
float voltage,phValue,temperature;
DFRobot_PH ph;
DFRobot_ADS1115 ads(&Wire);
DS18B20 ds18b20(32);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);  
  Serial.println("Ready");
  ds18b20.Init();
  ph.begin();
}

float readTemperature()
{
  ds18b20.update_temperature();
  Serial.print("Temperatura: ");
  temperature = ds18b20.get_temperature();
  Serial.print(temperature);
  //Serial.printf("%.4f ºC ", temp);
  Serial.println(" ºC");
  delay(500);
  return temperature;
}

void loop() 
{
  static unsigned long timepoint = millis();
  if(millis()-timepoint>1000U)  //time interval: 1s
  {
    timepoint = millis();
    if (ads.checkADS1115())
    {
      voltage = float(ads.readVoltage(0));   // read the voltage(VOLTS)
    }
    Serial.println("Voltage: ");
    Serial.print(voltage);
    temperature = readTemperature();          // read your temperature sensor to execute temperature compensation
    phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
    Serial.println("Temperature: ");
    Serial.print(temperature,1);
    Serial.print(" ºC  EC: ");
    if (phValue > 0 || phValue <14)
    {
      Serial.print(phValue,4);
    }
    else if (phValue <= 0)
    {
      phValue = 0;
      Serial.print(phValue,2);
    }
    else
    {
      phValue = 0;
      Serial.print(phValue,4);
    }
    Serial.print(" (adimensional)");
  }
  //ph.calibration(voltage,temperature);           // calibration process by Serail CMD
}
