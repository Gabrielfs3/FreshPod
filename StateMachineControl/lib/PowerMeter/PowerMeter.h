#ifndef PowerMeter_h
#define PowerMeter_h

extern int CF_PIN,CF1_PIN,SEL_PIN,relay_PIN;
extern char CURRENT_MODE;

class PowerMeter{
    public:
        PowerMeter(int, int, int, char ,int);
        void Init();
        void power_task(int);
        float return_energy();
        long int return_TS_power();
    private:
        unsigned int _cf_pin;
        unsigned int _cf1_pin;
        unsigned int _sel_pin;
        unsigned char _current_mode_;
        unsigned int _relay_;
};
#endif