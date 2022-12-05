#ifndef sensorec_h
#define sensorec_h

class sensorEC
{
    public:
        sensorEC(int pin);
        void init();
        float get_EC();
    
    private:
        float ec_Value;
        int pin_ec;
        unsigned long int avgValue;                         //Store the average value of the sensor feedback
        float b;
        int buf[10],e_c;
};

#endif