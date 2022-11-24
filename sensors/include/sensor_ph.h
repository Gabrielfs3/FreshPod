#ifndef sensorph_h
#define sensorph_h

class sensorpH
{
    public:
        sensorpH(int pin);
        void init();
        float get_pH();
    
    private:
        float ph_Value;
        int pin_ph;
        unsigned long int avgValue;                         //Store the average value of the sensor feedback
        float b;
        int buf[10],ph;
};

#endif