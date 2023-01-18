#ifndef LuminositySensor_h
#define LuminositySensor_h

class LuminositySensor{
    public:
        LuminositySensor(int, int);
        void Init();
        float get_luminosity();
        
        unsigned int  _SCL;
        unsigned int _SCA;
    private:

};
#endif