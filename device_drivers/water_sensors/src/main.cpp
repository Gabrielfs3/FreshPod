#include <Arduino.h>
#include "sensor_ph.h"
#include "sensor_ec.h"
#include "sensor_o2.h"
#include "water_sensors.h"

int sensor_pin_ph = 33;
int sensor_pin_o2= 32;
int sensor_pin_ec = 35;

float o2, ph, ec;

water_sensors water(sensor_pin_ph, sensor_pin_o2, sensor_pin_ec);

void setup()
{
  Serial.begin(115200);                           //Init serial port and configure baudrate to 115200 bps
  Serial.println("Ready!");                       //Test the serial monitor
  water.init();                                //Initialize all 3 functions
}

void loop()
{
  water.water_sensors_tasks();

  Serial.print("pH: ");
  ph = water.return_pH();
  Serial.print(water.return_pH());              //Print pH value
  Serial.print(" (adimensional)  ");

  Serial.print("O2: ");
  o2 = water.return_O2();
  Serial.print(water.return_O2());              //Print O2 value
  Serial.print(" (mg/L)  ");

  Serial.print("EC: ");
  ec = water.return_EC();
  if (ec < 1 || ec > 15)
  {
      Serial.print("Valor fora da gama recomendada! ");
  }
  Serial.print(ec);                             //Print EC value
  Serial.println(" (ms/cm)");
}