#include <Arduino.h>
#include "sensor_ph.h"
#include "sensor_ec.h"
#include "sensor_o2.h"

sensorpH sensorph(14);
sensorO2 sensoro2(15);
sensorEC sensorec(16);

void setup()
{
  Serial.begin(115200);                           //Init serial port and configure baudrate to 115200 bps
  Serial.println("Ready!");                       //Test the serial monitor
  sensorph.init();                                //Initialize all 3 functions
  sensoro2.init();
  sensorec.init();
}

void loop()
{
  sensorph.volt_pH();
  Serial.println("pH: ");
  Serial.println(sensorph.get_pH());              //Print pH value
  delay(100);
  /*sensoro2.volt_O2();
  Serial.println("O2: ");
  Serial.println(sensoro2.get_O2());              //Print O2 value
  delay(100);
  sensorec.volt_EC();
  Serial.println("EC: ");
  Serial.println(sensorec.get_EC());              //Print EC value
  delay(100);*/
}