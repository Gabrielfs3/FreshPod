#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "sensor_ph.h"
#include "sensor_ec.h"
#include "sensor_o2.h"
#include "Wifi_esp32.h"

sensorpH sensorph(33);
sensorO2 sensoro2(32);
sensorEC sensorec(35);

const char* rede = "Vodafone-624E88";
const char* password = "xGJXZcg7Jd";
const char* brokeruser ="admin";
const char* brokerpass = "admin";
const char* broker = "192.168.1.122";
const char* outTopicph = "/pH";
const char* outTopico2 = "/o2";
const char* outTopicec = "/ec";
const char* outTopicwater = "/water/sensor/measurement/PHO2CE";
char message_buffer[150];
float o2;
float ph;
float ec;

WiFiClient espclient;
PubSubClient client(espclient);
Wifi_esp32 wifi_esp32(rede,password);


void reconnect()
{
 while(!client.connected()) 
 {
  Serial.print("\nConnected to");
  Serial.println(broker);
  if(client.connect("400"))
  {
    Serial.print("\nConnected to");
    Serial.println(broker);
  }else
  {
    Serial.println("\nTrying connect again");
    delay(5000);
  }
 }
}

void setup()
{
  Serial.begin(115200);                           //Init serial port and configure baudrate to 115200 bps
  Serial.println("Ready!");                       //Test the serial monitor
  client.setServer(broker,1883);
  sensorph.init();                                //Initialize all 3 functions
  sensoro2.init();
  sensorec.init();
}

void loop()
{
  wifi_esp32.connect_live();
  if (!client.connected())
  {
    reconnect();
  }

  client.loop();
  ph = sensorph.get_pH();
  Serial.print("pH: ");
  Serial.print(sensorph.get_pH());              //Print pH value
  Serial.print(" (adimensional)  ");
  

  //dtostrf(ph,2,2,msg_out_ph);
 
  o2 = sensoro2.get_O2();
  Serial.print("O2: ");
  Serial.print(o2);              //Print O2 value
  Serial.print(" (mg/L)  ");
  

  //dtostrf(o2,2,2,msg_out_o2);

  Serial.print("EC: ");
  ec = sensorec.get_EC();
  if (ec <1 || ec > 15)
  {
      Serial.print("Valor fora da gama recomendada! ");
  }
  Serial.print(ec);              //Print EC value
  Serial.println(" (ms/cm)");

  //dtostrf(ec,2,2,msg_out_ec);
  sprintf(message_buffer,"{\"/water/sensor/measurement/PHO2CE\":{\"pH\":%f,\"o2\":%f,\"ce\":%f}}", ph, o2, ec);
  Serial.print(message_buffer);
  client.publish(outTopicwater,message_buffer);

  delay(10000);
}