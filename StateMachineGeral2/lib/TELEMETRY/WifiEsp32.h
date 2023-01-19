#ifndef WifiEsp32_h
#define WifiEsp32_h


class WifiEsp32{

public:
    WifiEsp32(const char* rede, const char* password);
    void connect();
    void connect_live();

private:
    const char* ssid;
    const char* passphrase;


};

#endif