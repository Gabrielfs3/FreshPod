#include <Arduino.h>
#include "sensor_ph.h"
#include "sensor_ec.h"
#include "sensor_o2.h"

sensorpH sensorph(25);
sensorO2 sensoro2(26);
sensorEC sensorec(27);

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
  Serial.print("pH: ");
  Serial.print(sensorph.get_pH());              //Print pH value
  Serial.print(" (adimensional)  ");
  delay(1000);
  Serial.print("O2: ");
  Serial.print(sensoro2.get_O2());              //Print O2 value
  Serial.print(" (mg/L)  ");
  delay(1000);
  Serial.print("EC: ");
  if (sensorec.get_EC() <1 || sensorec.get_EC() > 15)
  {
      Serial.print("Valor fora da gama recomendada! ");
  }
  Serial.print(sensorec.get_EC());              //Print EC value
  Serial.println(" (ms/cm)");
  delay(1000);
}