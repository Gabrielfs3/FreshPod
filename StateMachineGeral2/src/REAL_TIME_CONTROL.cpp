#include "WiFi.h"
#include <Arduino.h>
#include <ESP32Ping.h>
#include "REAL_TIME_CONTROL.h"
#include <PubSubClient.h>
#include <Actuators.h>

//WiFiClient espClient;
WiFiClient net;
PubSubClient client(net);


enum struct trafStatesRealTime : int        //estados
        {
        STAND_BY_REAL_TIME,
        ACTUATOR_OUTPUT_UPDATE,
        };



trafStatesRealTime RealTimeState;


struct  Eventos
    {
        boolean subscribed_topics=true;
        boolean Updated_actuator_states=false;
        boolean msg_received=false;
    };

trafStatesRealTime Real_time;       
Eventos evento_rt;

char* msg_LEDs,*msg_Fans, *msg_Pump;

void callback(char* topic, byte* payload, unsigned int length) ;


void Real_Time::Init(){
        Serial.println("Real time init");
        client.setCallback(callback);
        client.subscribe("/Actuadores/RealTime/Control/LEDs");
        client.subscribe("/Actuadores/RealTime/Control/Fans");
        client.subscribe("/Actuadores/RealTime/Control/Pump");
       
        
}



void Real_Time::Task(){
       
       //processar todos os eventos
        Serial.println("Real time task");

       client.loop();
        if(evento_rt.subscribed_topics == true)
        {
            RealTimeState=trafStatesRealTime::STAND_BY_REAL_TIME;
            evento_rt.subscribed_topics= false;
        }

        else if(evento_rt.Updated_actuator_states== true)
        {
            RealTimeState=trafStatesRealTime::STAND_BY_REAL_TIME;
            evento_rt.Updated_actuator_states= false;
        }

        else if(evento_rt.msg_received== true)
        {
            RealTimeState=trafStatesRealTime::ACTUATOR_OUTPUT_UPDATE;
            evento_rt.msg_received= false;
        }


        switch (RealTimeState)     //executa o estado atual 
        {
        case trafStatesRealTime::ACTUATOR_OUTPUT_UPDATE:
        Serial.println("actuator output update");

                  if(msg_LEDs=="1"){
                      setLED(true);
                      Serial.println("liga leds");
                    }
                  else if(msg_LEDs=="0")
                    {
                      setLED(false);
                      Serial.println("desliga leds");
                    }

                  if(msg_Pump=="1"){
                      setPump(true);
                      Serial.println("liga pump");
                    }
                  else if(msg_Pump=="0")
                    {
                      setPump(false);
                      Serial.println("desliga pump");
                    }

                  if(msg_Fans=="1"){
                      setFan(true);
                      Serial.println("liga fan");
                    }
                  else if(msg_Fans=="0")
                    {
                      setFan(false);
                      Serial.println("desliga fan");

                      evento_rt.Updated_actuator_states=true;
      }

            
            break;

        case trafStatesRealTime::STAND_BY_REAL_TIME:
                Serial.println("stand by real time");

              client.loop(); 
            break;

        
        
        default:
            break;
        }
    

}


void callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';
    String strTopic = String((char*)topic);
    evento_rt.msg_received=true;

    if (strTopic == "/Actuadores/RealTime/Control/LEDs") {
      msg_LEDs = (char*)payload;
      
    }

    if (strTopic == "/Actuadores/RealTime/Control/Fans") {
      msg_Fans = (char*)payload;
       
    } 

    if (strTopic == "/Actuadores/RealTime/Control/Pump") {
      msg_Pump = (char*)payload;
       
    } 

}
