#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
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
int monthDay;
int CurrentMonth;
int CurrentYear;
String CurrentMonthName;
String formattedDate;
String FormattedDay;
String CurrentDate;
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


WiFiClient espclient;
PubSubClient client(espclient);
Wifi_esp32 wifi_esp32(rede,password);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);


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
  timeClient.begin();                             //Initialize a NTPclient to get time
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
  timeClient.update(); // get the current date and time from the NTP server.
  time_t epochTime = timeClient.getEpochTime(); //number of seconds that have elapsed since January 1, 1970 (midnight GMT);

  struct tm *ptm = gmtime((time_t*)&epochTime); //The NTP Client doesn’t come with functions to get the date. So, we need to create a time structure (struct tm) and then, access its elements to get information about the date
  monthDay=ptm->tm_mday; //tm_mday dia do mes
  CurrentMonth=ptm->tm_mon+1; // tm_mon meses desde janeiro. Adicionamos 1 pq começa em 0 e assim o 1 corresponde a janeiro e 2 a fevereiro etc
  CurrentMonthName=months[CurrentMonth-1]; //subtraimos 1 pq o prieiro indice é zero
  CurrentYear=ptm->tm_year+1900; //tm_year anos desde 1900
  CurrentDate=String(monthDay) + '/' + String(CurrentMonth) + '/' + String(CurrentYear);

  client.loop();
  ph = sensorph.get_pH();
  formattedDate = timeClient.getFormattedTime();
  Serial.print("pH: ");
  Serial.print(ph);              //Print pH value
  Serial.print(" (adimensional)  ");
  Serial.print("Day: ");
  Serial.print(CurrentDate);
  Serial.print(" Hour: ");
  Serial.print(formattedDate);
  Serial.print("\n");
  

  //dtostrf(ph,2,2,msg_out_ph);
 
  o2 = sensoro2.get_O2();
  formattedDate = timeClient.getFormattedTime();
  Serial.print("O2: ");
  Serial.print(o2);              //Print O2 value
  Serial.print(" (mg/L)  ");
  Serial.print(" Day: ");
  Serial.print(CurrentDate);
  Serial.print(" Hour: ");
  Serial.print(formattedDate);
  Serial.print("\n");
  

  //dtostrf(o2,2,2,msg_out_o2);

  Serial.print("EC: ");
  ec = sensorec.get_EC();
  formattedDate = timeClient.getFormattedTime();
  if (ec <1 || ec > 15)
  {
      Serial.print("Valor fora da gama recomendada! ");
      Serial.print("Day: ");
      Serial.print(CurrentDate);
      Serial.print(" Hour: ");
      Serial.print(formattedDate);
      Serial.print("\n");
  }
  Serial.print(ec);              //Print EC value
  Serial.println(" (ms/cm)");
  Serial.print("Day: ");
  Serial.print(CurrentDate);
  Serial.print(" Hour: ");
  Serial.print(formattedDate);
  Serial.print("\n");

  //dtostrf(ec,2,2,msg_out_ec);
  sprintf(message_buffer,"{\"pH\":%f,\"o2\":%f,\"ce\":%f}", ph, o2, ec);
  //Serial.print(message_buffer);
  client.publish(outTopicwater,message_buffer);

  delay(10000);
}