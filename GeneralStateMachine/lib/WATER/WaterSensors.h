#ifndef WaterSensors_h
#define WaterSensors_h

#include "Arduino.h"

extern int sensor_pin_ph, sensor_pin_o2, sensor_pin_ec;

class WaterSensors{
    public:
        WaterSensors(int pin, int pinx, int piny);
        void init();
        void water_sensors_tasks();
        float return_ph();
        float return_o2 ();
        float return_ec ();
    private:
        float ph, o2, ec;
        int pin;
};

#endif