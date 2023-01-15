#include <stdbool.h>
#ifndef Actuators_h
#define Actuators_h

#include "Arduino.h"

    void Init_AllActuators();		//Inicializa todos os atuadores
    boolean Return_Fan();		//Retorna o estado das ventoinhas/leds/bomba de agua, 1 = on
    boolean Return_LED();
    boolean Return_Pump();

    void setPump(boolean value);	//Define o estado da ventoinha/leds,bomba de agua, 1 = on
    void setLED(boolean value);
    void setFan(boolean value);    
#endif