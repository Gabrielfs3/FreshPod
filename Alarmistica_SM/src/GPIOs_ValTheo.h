#include <Arduino.h>

//Power meter
    //------[HLW8012]------
    int relay_PIN =13;
    int SEL_PIN=15;
    int CF1_PIN=2;
    int CF_PIN =4;
    char CURRENT_MODE=LOW;       
//----------------------------------
//Actuator
    int FanPIN=25;
    int LEDPIN=26;
    int PumpPIN=27;
//----------------------------------

//----------[Valores teoricos]------------------------
int geral_consumption=0.11; //wh
int geral_power=11;
