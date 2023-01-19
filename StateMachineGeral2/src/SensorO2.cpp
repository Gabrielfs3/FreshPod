#include <Arduino.h>
#include "SensorO2.h"
#include <DS18B20.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <DFRobot_ADS1115.h>

DFRobot_ADS1115 ads_2(&Wire);
DS18B20 ds18b20_2(32);

#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution

#define CAL1_V (1600) //mv
#define CAL1_T (20)   //℃

const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};

int16_t readDO(uint32_t voltage_mv, uint8_t temperature_c)
{
  uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature_c - (uint32_t)CAL1_T * 35;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
}

void SensorO2::init()
{
  ds18b20_2.Init();
  ads_2.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);   // 0x48
  ads_2.setGain(eGAIN_ONE);   // 2/3x gain
  ads_2.setMode(eMODE_SINGLE);       // single-shot mode
  ads_2.setRate(eRATE_128);          // 128SPS (default)
  ads_2.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
  ads_2.init();
}

float SensorO2::get_o2()
{
  static unsigned long timepoint = millis();
  if(millis()-timepoint>1000U)  //time interval: 1s
  {
  timepoint = millis();
  if (ads_2.checkADS1115())
  {
    voltage = float(ads_2.readVoltage(2));   // read the voltage(VOLTS)
  }
  temperature = ds18b20_2.get_temperature();           // read your temperature sensor to execute temperature compensation
  //ecValue=ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
  ADC_Voltage = VREF * voltage / ADC_RES;
  o2Value = readDO(voltage, temperature);
  }
  return o2Value;
}