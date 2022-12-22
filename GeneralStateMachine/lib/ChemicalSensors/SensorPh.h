#ifndef SensorPh_h
#define SensorPh_h

class SensorPh
{
    public:
        SensorPh(int pin);
        void init();
        float get_ph();
    
    private:
        float ph_Value;
        int pin_ph;
        unsigned long int avgValue;                         //Store the average value of the sensor feedback
        float b;
        int buf[10],ph;
};

#endif