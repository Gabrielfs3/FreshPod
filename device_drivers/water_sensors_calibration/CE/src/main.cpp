#include <Arduino.h>
#include <DS18B20.h>
#include "DFRobot_EC.h"
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <DFRobot_ADS1115.h>

#define RES2 820.0
#define ECREF 200.0

//#define EC_PIN 33
float voltage,ecValue,temperature, _rawEC;
DFRobot_EC ec;
DFRobot_ADS1115 ads(&Wire);
DS18B20 ds18b20(32);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);  
  Serial.println("Ready");
  ds18b20.Init();
  ec.begin();
  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);   // 0x48
  ads.setGain(eGAIN_ONE);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);       // single-shot mode
  ads.setRate(eRATE_128);          // 128SPS (default)
  ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
  ads.init();
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
  //ecValue=ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
  _rawEC = 1000*voltage/RES2/ECREF;
  ecValue = voltage / (1.0+0.0185*(temperature-25.0));  //temperature compensation
  Serial.print("Temperature: ");
  Serial.print(temperature,1);
  Serial.print(" ºC  EC: ");
  if (ecValue > 0 || ecValue <20)
  {
    Serial.print(ecValue,4);
  }
  else if (ecValue <= 0)
  {
    ecValue = 0;
    Serial.print(ecValue,4);
  }
  else
  {
    ecValue = 0;
    Serial.print(ecValue,4);
  }
  Serial.println(" ms/cm");
  }
  //ec.calibration(voltage,temperature);          // calibration process by Serail CMD
}
