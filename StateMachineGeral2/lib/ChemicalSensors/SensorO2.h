#ifndef SensorO2_h
#define SensorO2_h


class SensorO2
{
    public:
        void init();
        float get_o2();
    
    private:
	 float voltage, o2Value, temperature, _rawEC, ADC_Voltage, ADC_Raw;
};

#endif