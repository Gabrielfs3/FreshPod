#ifndef environment_senso_h
#define environment_senso_h

#include "Arduino.h"

class environment{
    public:
        environment(int, int, int, int, int);
        void Init();
        bool return_Watter();
        bool return_Temp ();
    private:
        int yellow_pin;
        int type;
        int echoPin;
        int trigPin;
        int baudrate;
};

#endif