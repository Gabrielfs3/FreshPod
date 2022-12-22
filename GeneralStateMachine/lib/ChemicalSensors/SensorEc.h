#ifndef SensorEc_h
#define SensorEc_h

class SensorEc
{
    public:
        SensorEc(int pin);
        void init();
        float get_ec();
    
    private:
        float ec_Value;
        int pin_ec;
        unsigned long int avgValue;                         //Store the average value of the sensor feedback
        float b;
        int buf[10],e_c;
};

#endif