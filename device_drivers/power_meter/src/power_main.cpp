#include <Arduino.h>
#include "hlw8012p.h"


#define SERIAL_BAUDRATE                 115200

// GPIOs
#define RELAY_PIN                       12
#define SEL_PIN                         5
#define CF1_PIN                         13
#define CF_PIN                          14
#define CURRENT_MODE                    HIGH
#define use_interrupts                  false
#define _pulse_timeout                  500000


hlw8012p power_meter(CF_PIN,CF1_PIN,SEL_PIN,CURRENT_MODE,use_interrupts,_pulse_timeout);

float current;
float voltage;
float power;

void setup() 
{
    power_meter.Init();
    Serial.begin(SERIAL_BAUDRATE); // For testing
    Serial.print("\nTESTE MAIN POWER");
}

void loop() 
{
    delay(1000);
    Serial.print("\nCurrent?");
    current= power_meter.get_current();
    Serial.print(current,1);
    Serial.println(" A");

    delay(1000);
    Serial.print("\nvoltage?");
    voltage= power_meter.get_voltage();
    Serial.print(voltage,1);
    Serial.println(" V");
    
    delay(1000);
    Serial.print("\nActive Power?");
    power= power_meter.get_power();
    Serial.print(power,1);
    Serial.println(" w");
}