#ifndef EnvironmentSensors_h
#define EnvironmentSensors_h

#include "Arduino.h"

class environment{
    public:
        environment(int, int, int, int, int);
        void Init();
        void environment_sensor_tasks();
        float return_luminousity();
        float return_temperature ();
        float return_humidity();
    private:
        int yellow_pin;
        int type;
        int scl;
        int sca;
        int baudrate;
};

#endif