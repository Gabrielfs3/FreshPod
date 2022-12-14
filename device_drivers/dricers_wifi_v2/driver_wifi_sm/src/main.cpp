#include <Arduino.h>
#include "../include/WiFi_Driver.h"

const char* rede = "A51";
const char* password = "qwerty123";


WiFi_Driver wifi_esp(rede,password);

void setup() {
  Serial.begin(115200); 
  wifi_esp.Init();
}

void loop() {
  wifi_esp.Task();
  Serial.println(wifi_esp.return_wifi_state ());
}