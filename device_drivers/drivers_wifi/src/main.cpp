#include <Arduino.h>
#include </home/jorge/Documents/PlatformIO/Projects/esp32s2_test/include/Wifi_esp32.h>
#include </home/jorge/Desktop/PI_FreshPod/FreshPod/device_drivers/drivers_wifi/include/DS18B20.h>

const char* rede = "A51";
const char* password = "qwerty123";


Wifi_esp32 wifi_esp32(rede,password);

DS18B20 ds18b20(5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  
  Serial.println("Ready");   
  //wifi_esp32.connect();
  //Serial.println("ligado"); 
  Serial.println("inicio "); 
  ds18b20.Init();

}

void loop() {
    // put your main code here, to run repeatedly:
    //wifi_esp32.connect_live();
    ds18b20.update_temperature();
    Serial.println("temperatura"); 
    Serial.println(ds18b20.get_temperature()); 
    delay(500);
}