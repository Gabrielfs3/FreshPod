#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>
#include <Wifi_esp32.h>

const char* ssid = "Vodafone-CAD4A5";
const char* pass = "Dje4dUNe8h3Ffxz8";

WiFiClient net;
MQTTClient client;

Wifi_esp32 wifi_esp32(ssid,pass);
void connect() {                                                            //conect to mqtt broker
  wifi_esp32.connect();
  while (!client.connect("TESTETESTE", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected to mqtt broker!");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(115200);
  client.begin("public.cloud.shiftr.io", net);                //broker mqtt
  client.onMessage(messageReceived);
  connect();
}

void loop() {
  client.loop();                 //retorna p o loop se o client n estiver conectado
  if (!client.connected()) 
  {
    connect();                    //volta a conectar
  }
  
  client.publish("/teste", "publish");
  client.subscribe("teste subscribe");
  delay(1000);

}

