#ifndef power_meter_h
#define power_meter_h

class power_meter{
    public:
        power_meter(int, int, int, char ,bool,long);
        void Init();
        void power_task();
        double * return_energy();
    private:
        unsigned int _cf_pin;
        unsigned int _cf1_pin;
        unsigned int _sel_pin;
        unsigned char _current_mode_;
        bool _use_interrupts;
        unsigned long _pulse_timeout_;
};
#endif