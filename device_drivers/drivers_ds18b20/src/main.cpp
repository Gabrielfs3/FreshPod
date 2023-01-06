#include <Arduino.h>
#include <DS18B20.h>

DS18B20 ds18b20(32);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);  
  Serial.println("Ready");
  ds18b20.Init();
}

void loop() 
{
    float temp;
    ds18b20.update_temperature();
    Serial.print("Temperatura: ");
    temp = ds18b20.get_temperature();
    Serial.print(temp);
    //Serial.printf("%.4f ºC ", temp);
    Serial.println(" ºC");
    delay(500);
}