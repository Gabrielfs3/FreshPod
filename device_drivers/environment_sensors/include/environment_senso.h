#ifndef environment_senso_h
#define environment_senso_h

#include "Arduino.h"

class environment{
    public:
        environment(int, int, int, int, int);
        void Init();
        float return_Watter();
        float return_Temp ();
        float return_humi ();
    private:
        int yellow_pin;
        int type;
        int echoPin;
        int trigPin;
        int baudrate;
};

#endif