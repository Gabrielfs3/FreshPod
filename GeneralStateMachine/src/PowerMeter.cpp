#include <Arduino.h>
#include "PowerMeter.h"
#include "HLW8012_driver.h"


#define SERIAL_BAUDRATE                 115200

//global variables
static float power, convert, consumo=0.0, return_consumo=0.0;
static float ener;
int sampling;
//define class
HLW8012_driver hlw8012pm;

//define functions
void energy();

PowerMeter::PowerMeter(int cf, int cf1, int sel, char current_mode,int _relay)
{
    _cf_pin=cf;
    _cf1_pin=cf1;
    _sel_pin=sel;
    _current_mode_=current_mode;
    _relay_=_relay;
}

void PowerMeter::Init()
{
    Serial.begin(SERIAL_BAUDRATE);
    hlw8012pm.Init(_cf_pin,_cf1_pin,_sel_pin, _current_mode_,_relay_);
}

void PowerMeter::power_task(int time)
{   
    sampling=time;
    power=hlw8012pm.get_power();
    //ener=hlw8012pm.get_Energy();
    energy();
}

void energy()
{   
    power=11;//for testing "Potencia(W)"

    convert=((power)*(sampling/1000.0))/(60.0*60.0); 
    //convert=ener;
    consumo=(convert+consumo);
    //Serial.println(consumo,3);  //debug
    //Serial.println(ener,3);  //debug
}


float PowerMeter::return_energy()
{   
    //hlw8012pm.reset_energy();
    return_consumo=consumo;
    consumo=0;//reset consumo
    return return_consumo;
} 