#include "WiFi.h"
#include <Arduino.h>
#include <ESP32Ping.h>
#include "WiFi_Driver.h"

//WiFiClient espClient;
enum struct trafStatesWiFi : int        //estados
        {
        AP_CONNECTED,
        INTERNET_CONNECTED,
        DISCONNECTED
        };


struct  Eventos
    {
        boolean WiFi_conneccted=true;
        boolean internet_connected=false;
        boolean no_ping=false;
        boolean no_ap_connection=false;
        boolean ap_connection=false;
    };

trafStatesWiFi WiFi_State;       
Eventos evento;

int test_wifi_sample=10000;
int lastPing=0;
int tempo_int=0;
bool ping_success;
unsigned long previousMillis_dis = 0;
unsigned long interval_dis = 15000;
unsigned long currentMillis_dis = millis();


WiFi_Driver::WiFi_Driver(const char* rede, const char* password){
        ssid=rede;
        passphrase=password;
		WiFi.mode(WIFI_MODE_STA);
         
}
void WiFi_Driver::Init(){
        WiFi.begin(ssid, passphrase);
        
        while (WiFi.status() != WL_CONNECTED) 
        {
            delay(5);
           // Serial.print(".");
        }
        //Serial.println("IP address: ");
        //Serial.println(WiFi.localIP());
        
}

const char* WiFi_Driver::return_wifi_state(){

    const char* stateStr[] = {"AP_CONNECTED", "INTERNET_CONNECTED", "DISCONNECTED"};
   // Serial.println(stateStr[WiFi_State_global]);
    return stateStr[WiFi_State_global];
}



void WiFi_Driver::Task(){
       
       //processar todos os eventos
        if(evento.WiFi_conneccted == true)
        {
            WiFi_State=trafStatesWiFi::AP_CONNECTED;
            evento.WiFi_conneccted = false;
        }

        else if(evento.internet_connected== true)
        {
            WiFi_State=trafStatesWiFi::INTERNET_CONNECTED;
            evento.internet_connected= false;
        }

        else if(evento.no_ap_connection== true)
        {
            WiFi_State=trafStatesWiFi::DISCONNECTED;
            evento.no_ap_connection= false;
        }

        else if(evento.no_ping== true)
        {
            WiFi_State=trafStatesWiFi::AP_CONNECTED;
            evento.no_ping= false;
        }

        else if(evento.ap_connection== true)
        {
            WiFi_State=trafStatesWiFi::AP_CONNECTED;
            evento.ap_connection= false;
        }

        WiFi_State_global=(int) WiFi_State;
      //  Serial.println(WiFi_State_global);

        switch (WiFi_State)     //executa o estado atual 
        {
        case trafStatesWiFi::AP_CONNECTED:
            Serial.println("AP");
            if (WiFi.status() != WL_CONNECTED){

                evento.no_ap_connection=true;
                break;
            }
            
            ping_success = Ping.ping("8.8.8.8", 3);

            if(ping_success){
                evento.internet_connected=true;
            }

            break;

        case trafStatesWiFi::INTERNET_CONNECTED:
          //  Serial.println("INT");
            tempo_int=millis();
            if(tempo_int-lastPing>= test_wifi_sample)
                    {
                        ping_success = Ping.ping("8.8.8.8", 3);
                        if(!ping_success)
                            {
                                evento.no_ping=true;
                            }
                        lastPing=millis();
                    }
            break;

        case trafStatesWiFi::DISCONNECTED:
            //Serial.println("DIS");   
        	//WiFi.disconnect();
			//WiFi.begin(ssid, passphrase);
            currentMillis_dis = millis();
                
            if (currentMillis_dis - previousMillis_dis >=interval_dis) {
                    WiFi.disconnect();
                    WiFi.reconnect();
                    previousMillis_dis = currentMillis_dis;
                }

			if(WiFi.status() == WL_CONNECTED)
            {
		    evento.ap_connection=true;	
            }		          
            break;
        
        default:
            break;
        }
    

}

