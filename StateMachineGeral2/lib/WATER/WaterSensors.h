#ifndef WaterSensors_h
#define WaterSensors_h

#include "Arduino.h"

extern int sensor_pin_pin_echo, sensor_pin_pin_trig;

class WaterSensors{
    public:
        WaterSensors(int, int);
        void init();
        void water_sensors_tasks();
        float return_ph();
        float return_o2 ();
        float return_ec ();
        float return_water_level();
	  float return_temp();
        long int return_TS_ph();
        long int return_TS_o2();
        long int return_TS_ec();
        long int return_TS_water_level();
	  long int return_TS_temp();
    private:
        int echo_Pin, trig_Pin;
        float ph, o2, ec, water_level, temp;
        int pin;
};

#endif