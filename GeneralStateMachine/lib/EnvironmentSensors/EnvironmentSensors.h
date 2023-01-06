#ifndef EnvironmentSensors_h
#define EnvironmentSensors_h

#include "Arduino.h"


extern int  yellow_pin,DHTTYPE,_SCL,_SDA,baudrate;

class environment{
    public:
        environment(int, int, int, int, int);
        void Init();
        void environment_sensor_tasks();
        float return_luminousity();
        float return_temperature ();
        float return_humidity();
        long int return_TS_lumi();
        long int return_TS_temp();
        long int return_TS_humi();

    private:
        int yellow_pin;
        int type;
        int scl;
        int sda;
        int baudrate;
};

#endif