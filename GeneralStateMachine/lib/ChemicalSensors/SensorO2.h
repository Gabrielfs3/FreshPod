#ifndef SensorO2_h
#define SensorO2_h


class SensorO2
{
    public:
        SensorO2(int pin);
        void init();
        float get_o2();
    
    private:
        float o2_Value;
        int pin_o2;
        unsigned long int avgValue;                         //Store the average value of the sensor feedback
        float b;
        int buf[10],o_2;
};

#endif