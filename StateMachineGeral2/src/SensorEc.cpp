#include <Arduino.h>
#include "SensorEc.h"
#include <DS18B20.h>
#include "DFRobot_EC.h"
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <DFRobot_ADS1115.h>


#define RES2 820.0
#define ECREF 200.0

DFRobot_EC ec;
DFRobot_ADS1115 ads(&Wire);
DS18B20 ds18b20(32);

void SensorEc::init()
{
  ds18b20.Init();
  ec.begin();
  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);   // 0x48
  ads.setGain(eGAIN_ONE);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);       // single-shot mode
  ads.setRate(eRATE_128);          // 128SPS (default)
  ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
  ads.init();
}

float SensorEc::get_ec()
{
  static unsigned long timepoint = millis();
  if(millis()-timepoint>1000U)  //time interval: 1s
  {
  timepoint = millis();
  if (ads.checkADS1115())
  {
    voltage = float(ads.readVoltage(0));   // read the voltage(VOLTS)
  }
  temperature = ds18b20.get_temperature();          // read your temperature sensor to execute temperature compensation
  //ecValue=ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
  _rawEC = 1000*voltage/RES2/ECREF;
  ecValue = voltage / (1.0+0.0185*(temperature-25.0));  //temperature compensation
  }
  return ecValue;
}