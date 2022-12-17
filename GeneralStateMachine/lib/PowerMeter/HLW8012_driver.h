#ifndef HLW8012_driver_h
#define HLW8012_driver_h

class HLW8012_driver{
    public:
       HLW8012_driver();
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