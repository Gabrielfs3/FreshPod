#ifndef DS18B20_h
#define DS18B20_h


class DS18B20{

public:
    DS18B20(int pin);
    void Init();
    void update_temperature();
    float get_temperature();
private:
    int pin_temp;   
    float temperature;

};

#endif