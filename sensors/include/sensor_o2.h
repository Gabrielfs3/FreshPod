#ifndef sensoro2_h
#define sensoro2_h


class sensorO2
{
    public:
        sensorO2(int pin);
        void init();
        void volt_O2();
        float get_O2();
    
    private:
        float o2_Value;
        int pin_o2;
        unsigned long int avgValue;                         //Store the average value of the sensor feedback
        float b;
        int buf[10],temp;
};

#endif