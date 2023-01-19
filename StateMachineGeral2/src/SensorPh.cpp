#include <Arduino.h>
#include "SensorPh.h"
#include <DS18B20.h>
#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <DFRobot_ADS1115.h>


DFRobot_PH ph;
DFRobot_ADS1115 ads_1(&Wire);
DS18B20 ds18b20_1(32);


void SensorPh::init()
{
  ds18b20_1.Init();
  ph.begin();
  ads_1.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);   // 0x48
  ads_1.setGain(eGAIN_ONE);   // 2/3x gain
  ads_1.setMode(eMODE_SINGLE);       // single-shot mode
  ads_1.setRate(eRATE_128);          // 128SPS (default)
  ads_1.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
  ads_1.init();
}

float SensorPh::get_ph()
{
  static unsigned long timepoint = millis();
  if(millis()-timepoint>1000U)  //time interval: 1s
  {
    timepoint = millis();
    if (ads_1.checkADS1115())
    {
      voltage = float(ads_1.readVoltage(1));   // read the voltage(VOLTS)
    }
    Serial.println("Voltage: ");
    Serial.print(voltage);
    temperature = ds18b20_1.get_temperature();           // read your temperature sensor to execute temperature compensation
    phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
   }
   return phValue;
}