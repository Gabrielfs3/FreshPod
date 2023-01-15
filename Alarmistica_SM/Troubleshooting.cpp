#include "PowerMeter.h"
#include "Actuators.h"
#include "Trobleshooting.h"

//definir clases
PowerMeter  power_meter(CF_PIN,CF1_PIN,SEL_PIN,CURRENT_MODE,relay_PIN);

Trobleshooting::Trobleshooting()
{

}

void Trobleshooting::init()
{
    Serial.begin(115200);
    power_meter.Init();
    Init_AllActuators();
}


bool Trobleshooting::Trobleshooting_task()
{
  boolean alarm=0;
  float Wh_Pump_off,Wh_Pump_on,Wh_LEDS_off,Wh_LEDS_on;

  //---[watter pump] ----
  setPump(0); //Turn off=> 0

  //analisar e sguardar o valor do consumo

  setPump(1);//Turn on => 1
  //analisar e sguardar o valor do consumo

  /*
  if(Wh_Pump_on > Wh_Pump_off)
  {
    alarm=1;
  }
  */

  //---[LEDS]----
  setLED(0);//Turn off=> 0

  setLED(1);//Turn on=> 1
  /*
  if(Wh_LEDS_on > Wh_LEDS_off)
  {
    alarm=1;
  }
  */
  return alarm;
}
