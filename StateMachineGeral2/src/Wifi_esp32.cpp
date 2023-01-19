#include "WiFi.h"
#include <Arduino.h>
#include "WifiEsp32.h"

//WiFiClient espClient;

WifiEsp32::WifiEsp32(const char* rede, const char* password){
        ssid=rede;
        passphrase=password;
		WiFi.mode(WIFI_MODE_STA);
         
}
void WifiEsp32::connect(){
        WiFi.begin(ssid, passphrase);
        
        while (WiFi.status() != WL_CONNECTED) 
        {
            delay(100);
            Serial.print(".");
        }
        //Serial.println("IP address: ");
        //Serial.println(WiFi.localIP());        
}

void WifiEsp32::connect_live(){
    if(WiFi.status() != WL_CONNECTED)
        {
            Serial.println("sem wifi");
            WifiEsp32::connect();
        }

}