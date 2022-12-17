#ifndef water_sensors_h
#define water_sensors_h

#include "Arduino.h"

extern int sensor_pin_ph, sensor_pin_o2, sensor_pin_ec;

class water_sensors{
    public:
        water_sensors(int pin, int pinx, int piny);
        void init();
        void water_sensors_tasks();
        float return_pH();
        float return_O2 ();
        float return_EC ();
    private:
        float ph, o2, ec;
        int pin;
};

#endif