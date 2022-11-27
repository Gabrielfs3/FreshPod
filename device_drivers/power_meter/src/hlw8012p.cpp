#include <Arduino.h>
#include "HLW8012.h"
#include "hlw8012p.h"

 // GPIOs
#define RELAY_PIN   12
#define SERIAL_BAUDRATE                 115200
// These are the nominal values for the resistors in the circuit
#define CURRENT_RESISTOR                0.001
#define VOLTAGE_RESISTOR_UPSTREAM       ( 5 * 470000 ) // Real: 2280k
#define VOLTAGE_RESISTOR_DOWNSTREAM     ( 1000 ) // Real 1.009k
#define UPDATE_TIME                     2000

HLW8012 hlw8012;

hlw8012p::hlw8012p(int cf, int cf1, int sel, char current_mode,bool interrupts,long pulse_timeout)
{
    cf_pin=cf;
    cf1_pin=cf1;
    sel_pin=sel;
    _current_mode=current_mode;
    use_interrupts=interrupts;
    _pulse_timeout=pulse_timeout;
}


void hlw8012p::Init()
{  
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);
    hlw8012.begin(cf_pin, cf1_pin, sel_pin,  _current_mode, use_interrupts, _pulse_timeout);
    hlw8012.setResistors(CURRENT_RESISTOR, VOLTAGE_RESISTOR_UPSTREAM, VOLTAGE_RESISTOR_DOWNSTREAM);

    // Show default (as per datasheet) multipliers
    Serial.begin(SERIAL_BAUDRATE); // For testing
    Serial.print("[HLW] Default current multiplier : "); Serial.println(hlw8012.getCurrentMultiplier());
    Serial.print("[HLW] Default voltage multiplier : "); Serial.println(hlw8012.getVoltageMultiplier());
    Serial.print("[HLW] Default power multiplier   : "); Serial.println(hlw8012.getPowerMultiplier());
    Serial.println();
}

float hlw8012p::get_current()
{
    float current;
    static unsigned long last = millis();
    if ((millis() - last) > UPDATE_TIME) 
    {
        last = millis();
        current=hlw8012.getCurrent();
        Serial.println("\ndentro do if current");
    }
    //current=1.36;
    return current;
    // When not using interrupts we have to manually switch to current or voltage monitor
    // This means that every time we get into the conditional we only update one of them
    // while the other will return the cached value.
    hlw8012.toggleMode();
}

float hlw8012p::get_voltage()
{
    float voltage;
    static unsigned long last = millis();
    if ((millis() - last) > UPDATE_TIME) 
    {
        last = millis();
        voltage=hlw8012.getVoltage();
         Serial.println("\ndentro do if voltage");
    }
    return voltage;
    // When not using interrupts we have to manually switch to current or voltage monitor
    // This means that every time we get into the conditional we only update one of them
    // while the other will return the cached value.
    hlw8012.toggleMode();
}

float hlw8012p::get_power()
{
    float power;
    static unsigned long last = millis();
    if ((millis() - last) > UPDATE_TIME) 
    {
        last = millis();
        power=hlw8012.getActivePower();
         Serial.println("\ndentro do if power");
    }
    return power;
    // When not using interrupts we have to manually switch to current or voltage monitor
    // This means that every time we get into the conditional we only update one of them
    // while the other will return the cached value.
    hlw8012.toggleMode();
}