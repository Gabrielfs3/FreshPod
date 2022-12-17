#ifndef power_meter_h
#define power_meter_h

extern int CF_PIN,CF1_PIN,SEL_PIN,relay_PIN;
extern char CURRENT_MODE;

class power_meter{
    public:
        power_meter(int, int, int, char ,int);
        void Init();
        void power_task(int);
        float return_energy();
    private:
        unsigned int _cf_pin;
        unsigned int _cf1_pin;
        unsigned int _sel_pin;
        unsigned char _current_mode_;
        unsigned int _relay_;
};
#endif