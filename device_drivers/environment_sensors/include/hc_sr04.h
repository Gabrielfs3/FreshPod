#ifndef hc_sr04_h
#define hc_sr04_h


#include "Arduino.h"

class hc_sr04{
    public:
        hc_sr04(int, int);
        void Init();
        float get_dist();
    private:
        int echoPin;
        int trigPin;
};

#endif