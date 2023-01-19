#ifndef HC_SR04_driver_h
#define HC_SR04_driver_h

#include "Arduino.h"

class HC_SR04_driver{
    public:
        HC_SR04_driver(int, int);
        void Init();
        float get_dist();
    private:
        int echoPin;
        int trigPin;
};

#endif