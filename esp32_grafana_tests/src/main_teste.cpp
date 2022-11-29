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
const char* value = "7";
char msg_out_ph[20];
char msg_out_o2[20];
char msg_out_ec[20];

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
  
  Serial.print("pH: ");
  Serial.print(sensorph.get_pH());              //Print pH value
  Serial.print(" (adimensional)  ");
  float ph = sensorph.get_pH();

  dtostrf(ph,2,2,msg_out_ph);
  client.publish(outTopicph, msg_out_ph);
  delay(1000);

  Serial.print("O2: ");
  Serial.print(sensoro2.get_O2());              //Print O2 value
  Serial.print(" (mg/L)  ");
  float o2 = sensoro2.get_O2();

  dtostrf(o2,2,2,msg_out_o2);
  client.publish(outTopico2, msg_out_o2);
  delay(1000);

  Serial.print("EC: ");
  float ec = sensorec.get_EC();
  if (ec <1 || ec > 15)
  {
      Serial.print("Valor fora da gama recomendada! ");
  }
  Serial.print(ec);              //Print EC value
  Serial.println(" (ms/cm)");

  dtostrf(ec,2,2,msg_out_ec);
  client.publish(outTopicec, msg_out_ec);
  delay(1000);
}