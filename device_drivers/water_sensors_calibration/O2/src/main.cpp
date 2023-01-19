#include <Arduino.h>
#include <DS18B20.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <DFRobot_ADS1115.h>

float voltage,o2Value,temperature, _rawEC, ADC_Voltage, ADC_Raw;
DFRobot_ADS1115 ads(&Wire);
DS18B20 ds18b20(32);

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

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);  
  Serial.println("Ready");
  ds18b20.Init();
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
  temperature = readTemperature();          // read your temperature sensor to execute temperature compensation
  //ecValue=ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
  ADC_Voltage = VREF * voltage / ADC_RES;
  Serial.print("Temperature:\t");
  Serial.print(temperature,1);
  Serial.print(" ºC\tADC_RAW:\t");
  Serial.print(voltage);
  Serial.print(" mV\tADC_Voltage:\t");
  Serial.print(ADC_Voltage);
  Serial.print(" mV\tO2:\t");
  o2Value = readDO(voltage, temperature);
  if (o2Value > 0 || o2Value <20)
  {
    o2Value = o2Value/1000;
    Serial.print(o2Value,4);
  }
  else if (o2Value <= 0)
  {
    o2Value = 0;
    Serial.print(o2Value,4);
  }
  else
  {
    o2Value = 0;
    Serial.print(o2Value,4);
  }
  Serial.println(" mg/L");
  }
}