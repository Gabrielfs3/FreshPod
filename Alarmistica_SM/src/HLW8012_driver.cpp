#include <Arduino.h>
#include "HLW8012.h"
#include "HLW8012_driver.h"

#define SERIAL_BAUDRATE                 115200

// These are the nominal values for the resistors in the circuit
#define CURRENT_RESISTOR                0.001
#define VOLTAGE_RESISTOR_UPSTREAM       ( 5 * 470000 ) // Real: 2280k
#define VOLTAGE_RESISTOR_DOWNSTREAM     ( 1000 ) // Real 1.009k

// Check values every 10 seconds
//#define UPDATE_TIME                     1000

//define class
HLW8012 hlw8012;

//global variables
int CF_pin,CF1_pin;

HLW8012_driver::HLW8012_driver()
{

}

void setInterrupts() ;

void HLW8012_driver::Init(int CF, int CF1, int SEL, char mode,int relay)
{  
    _CF=CF;
    _CF1=CF1;
    _SEL=SEL;
    _mode=mode;
    RELAY_PIN=relay;

    //Passage for public variables
    CF_pin=CF;
    CF1_pin=CF1;

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);

    hlw8012.begin(_CF, _CF1, _SEL,_mode, true);

    hlw8012.setResistors(CURRENT_RESISTOR, VOLTAGE_RESISTOR_UPSTREAM, VOLTAGE_RESISTOR_DOWNSTREAM);
    setInterrupts();

    /* 

    //###########[ debug prints ]#########################
    Serial.begin(SERIAL_BAUDRATE); // For testing
    Serial.println("======[using interrupts]====");
    // Show default (as per datasheet) multipliers
    Serial.begin(SERIAL_BAUDRATE); 
    Serial.print("[HLW] Default current multiplier : "); Serial.println(hlw8012.getCurrentMultiplier());
    Serial.print("[HLW] Default voltage multiplier : "); Serial.println(hlw8012.getVoltageMultiplier());
    Serial.print("[HLW] Default power multiplier   : "); Serial.println(hlw8012.getPowerMultiplier());
    Serial.println();
    //####################################################~
    
    */
}

//----------interruptions ----------

// When using interrupts we have to call the library entry point
// whenever an interrupt is triggered
void ICACHE_RAM_ATTR hlw8012_cf1_interrupt() 
{
    hlw8012.cf1_interrupt();
}
void ICACHE_RAM_ATTR hlw8012_cf_interrupt() 
{
    hlw8012.cf_interrupt();
}

// Library expects an interrupt on both edges
void setInterrupts() 
{
    attachInterrupt(digitalPinToInterrupt(CF1_pin), hlw8012_cf1_interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(CF_pin), hlw8012_cf_interrupt, CHANGE);
}
//------------------------------------

double HLW8012_driver::get_current()
{
    return hlw8012.getCurrent();
}

double HLW8012_driver::get_voltage()
{
    return hlw8012.getVoltage();
}

double HLW8012_driver::get_power()
{
    return hlw8012.getActivePower();
}

double HLW8012_driver::get_Energy()
{
    return hlw8012.getEnergy();
}

void HLW8012_driver::reset_energy()
{
    hlw8012.resetEnergy();
}
