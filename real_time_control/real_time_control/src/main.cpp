#include <Arduino.h>
#include <Actuators.h>
#include <PubSubClient.h>
#include <REAL_TIME_CONTROL.h>


#include "../include/WiFi_Driver.h"

const char* rede = "A51";
const char* password = "qwerty123";

const char* mqttServer = "192.168.1.122";
const int mqttPort = 1883;
const char* mqttUser = "FreshPodMQTTuser";
const char* mqttPassword = "s";


WiFi_Driver wifi_esp(rede,password);
Real_Time   real_time;

void setup() {
  Serial.begin(115200);
  Init_AllActuators();
  wifi_esp.Init();
  real_time.Init();
 


  }


void loop() {
  wifi_esp.Task(); //executa as tarefas do wifi 
  Serial.println(wifi_esp.return_wifi_state ()); // print do estado atual
  real_time.Task();
}