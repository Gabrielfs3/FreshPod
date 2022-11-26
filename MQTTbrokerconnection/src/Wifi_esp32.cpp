#include "WiFi.h"
#include <Arduino.h>
#include "Wifi_esp32.h"

//WiFiClient espClient;

Wifi_esp32::Wifi_esp32(const char* rede, const char* password){
        ssid=rede;
        passphrase=password;
		WiFi.mode(WIFI_MODE_STA);
         
}
void Wifi_esp32::connect(){
        WiFi.begin(ssid, passphrase);
        
        while (WiFi.status() != WL_CONNECTED) 
        {
            delay(100);
            Serial.print(".");
        }
        //Serial.println("IP address: ");
        //Serial.println(WiFi.localIP());
        
}



void Wifi_esp32::connect_live(){
    if(WiFi.status() != WL_CONNECTED)
        {
            Serial.println("sem wifi");
            Wifi_esp32::connect();
        }

}