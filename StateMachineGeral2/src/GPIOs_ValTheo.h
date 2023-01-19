
#include <Arduino.h>
//Environment Sensors
    //------[DHT11]------
    int  yellow_pin=19;
    int DHTTYPE=11; //modelo do equipamento 
    //------[luminousity]------
   // int _SCL=22;
   // int _SDA=21;
//----------------------------------

//Power meter
    //------[HLW8012]------
    int relay_PIN =13;
    int SEL_PIN=15;
    int CF1_PIN=2;
    int CF_PIN =4;
    char CURRENT_MODE=LOW;      
//----------------------------------

//Water Sensors
    int sensor_pin_ph = 33;
    int sensor_pin_o2= 32;
    int sensor_pin_ec = 35;
    //------[HC_SR04]------
    int sensor_pin_pin_echo = 12;
    int sensor_pin_pin_trig = 14;
    //------[DS18B20]------
    int pin_temp = 34; //não é este o pino
//----------------------------------


//----------------------------------