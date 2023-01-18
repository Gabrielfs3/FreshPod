#ifndef DHT11_driver_h
#define DHT11_driver_h

#include "Arduino.h"

class DHT11class{
    public:
        DHT11class(int, int);
        void Init();
        float get_temperature();
        float get_humidity();
    private:
    int yellow_pin;
    int type;
};

#endif