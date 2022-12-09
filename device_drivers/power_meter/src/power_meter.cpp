#include <Arduino.h>
#include "power_meter.h"
#include "hlw8012p.h"


#define SERIAL_BAUDRATE                 115200

// Check values every 10 seconds
#define UPDATE_TIME                     1000

//global variables
static double current, power, bufer[15];
float rdm, consumo;
static int  reset_buf=0, i=0;

//define class
hlw8012p hlw8012pm;

//define functions
void energy();

power_meter::power_meter(int cf, int cf1, int sel, char current_mode,bool interrupts,long pulse_timeout)
{
    _cf_pin=cf;
    _cf1_pin=cf1;
    _sel_pin=sel;
    _current_mode_=current_mode;
    _use_interrupts=interrupts;
    _pulse_timeout_=pulse_timeout;
}

void power_meter::Init()
{
    Serial.begin(SERIAL_BAUDRATE);
    hlw8012pm.Init(_cf_pin,_cf1_pin,_sel_pin, _current_mode_,_use_interrupts,_pulse_timeout_);
}

void power_meter::power_task()
{
    static unsigned long last = millis();
    if ((millis() - last) > UPDATE_TIME) 
    {
        last = millis();
        //current=hlw8012pm.getCurrent();
        power=hlw8012pm.get_power();
        energy();
        /*
        if (use_interrupts==0) //false-not interruptions
        { 
            hlw8012pm.toggleMode(); 
            Serial.println("NOT INTERUPTS");       
        }
        */
    }

}

void energy()
{   
    if ( (reset_buf==1) )    //reseting energy
    {   
        for(int j=0; j<(i-1) ; j++)
        {
            bufer[j]={ };
        }
        reset_buf=0; i=0;         // initial conditions
    }

    rdm=random(0 ,100)/ 100.0; //for testing "Potencia(W)"
    consumo=(rdm/1000.0)*UPDATE_TIME ; //consumo em KWh
    //consumo=(power/1000.0)*UPDATE_TIME ;
    bufer[i]=consumo;
    i++;
}


double * power_meter::return_energy()
{
    reset_buf=1; //reset array
    return bufer;
} 