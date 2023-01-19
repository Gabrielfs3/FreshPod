#ifndef SensorEc_h
#define SensorEc_h

class SensorEc
{
    public:
        void init();
        float get_ec();
    
    private:
        float voltage,ecValue,temperature, _rawEC;
};

#endif