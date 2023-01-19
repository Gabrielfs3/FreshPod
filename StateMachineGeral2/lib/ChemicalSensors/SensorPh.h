#ifndef SensorPh_h
#define SensorPh_h

class SensorPh
{
    public:
        void init();
        float get_ph();
    
    private:
        float voltage,phValue,temperature;
};

#endif