#ifndef hlw8012p_h
#define hlw8012p_h

class hlw8012p{
    public:
       hlw8012p(int, int, int, char ,bool,long);
        void Init();
        float get_current();
        float get_voltage();
        float get_power();
    private:
        unsigned int cf_pin;
        unsigned int cf1_pin;
        unsigned int sel_pin;
        unsigned char _current_mode;
        bool use_interrupts;
        unsigned long _pulse_timeout;
};
#endif