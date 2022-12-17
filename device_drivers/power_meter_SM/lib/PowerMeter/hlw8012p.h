#ifndef hlw8012p_h
#define hlw8012p_h

class hlw8012p{
    public:
       hlw8012p();
        void Init(int, int, int, char,int);
        double get_current();
        double get_voltage();
        double get_power();
        double get_Energy();
        void reset_energy();
    private:
        unsigned int  _CF;
        unsigned int _CF1;
        unsigned int _SEL;
        unsigned char _mode;
        bool  _Interrupts;
        unsigned int  RELAY_PIN;
};
#endif