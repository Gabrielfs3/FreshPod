#ifndef Wifi_esp32_h
#define Wifi_esp32_h


class Wifi_esp32{

public:
    Wifi_esp32(const char* rede, const char* password);
    void connect();
    void connect_live();

private:
    const char* ssid;
    const char* passphrase;


};

#endif