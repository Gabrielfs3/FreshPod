#ifndef WiFi_Driver_h
#define WiFi_Driver_h



class WiFi_Driver{

public:
    
    WiFi_Driver(const char* rede, const char* password);
    void Init(); //inicia a ligaçao 
    void Task();
    const char* return_wifi_state();

private:
    int WiFi_State_global;
    const char* ssid;
    const char* passphrase;
    

};

#endif