#include <Arduino.h>
#include <LinkedList.h>
#include <StateMachine.h>
#include <Wifi_esp32.h>
#include "environment_senso.h"
#include "sensor_ph.h"
#include "sensor_ec.h"
#include "sensor_o2.h"
#include "DHT11_driver.h"
#include "water_sensors.h"

const char* rede = "A51";
const char* password = "qwerty123";

Wifi_esp32 wifi_esp32(rede,password);

const int STATE_DELAY = 1000;


unsigned long previous_time_water=millis();
unsigned long previous_time_temp_hum=millis();
unsigned long previous_time_lum=millis();

bool timeout_water_sensor=0;
bool timeout_temp_hum=0;
bool timeout_lum_waterlevel=0;

#define sampling_water 30000
#define sampling_temp_hum 15000
#define sampling_lum 900000

StateMachine machine = StateMachine();
Transition transiton;

void stateCHECK_CONNECTION();
void stateMQTT_STATE_STANDBY();
void stateMQTT_SEND();
void stateWATER_SENSORS_READ();
void stateENVIROMENT_SENSOR_READ();


bool transitionconn_alive();
bool transitionno_timeout();
bool transitionwater_sensor_event_readed();
bool transitionenviroment_sensor_event_readed();
//bool transitiontimeout_water_sensor();
//bool transitiontimeout_temp_hum();
//bool transitiontimeout_lum_waterlevel();
bool transitionmsg_sent();




State* CHECK_CONNECTION = machine.addState(&stateCHECK_CONNECTION); 
State* MQTT_STATE_STANDBY = machine.addState(&stateMQTT_STATE_STANDBY);
State* MQTT_SEND = machine.addState(&stateMQTT_SEND);
State* WATER_SENSORS_READ = machine.addState(&stateWATER_SENSORS_READ);
State* ENVIROMENT_SENSOR_READ =machine.addState(&stateENVIROMENT_SENSOR_READ);

//Water_Sensors_Inicialization
int sensor_pin_ph = 33;
int sensor_pin_o2= 32;
int sensor_pin_ec = 35;

//float o2, ph, ec;

water_sensors water(sensor_pin_ph, sensor_pin_o2, sensor_pin_ec);

//Environment_Sensors_Inicialization
//variables & GPIO
//------[DHT11]------
#define yellow_pin  27
#define DHTTYPE 11 //modelo do equipamento 

//------[HC_SR04]------
#define echoPin 12
#define trigPin 14

//------[geral]------
#define baudrate    115200
//---------------------
//float water,temp, humi;

environment ambi(yellow_pin, DHTTYPE, echoPin, trigPin, baudrate);



void setup() {
  wifi_esp32.connect();
  Serial.begin(115200);

  CHECK_CONNECTION->addTransition(&transitionconn_alive,MQTT_STATE_STANDBY);    // Transition to itself (see transition logic for details)
  MQTT_STATE_STANDBY->addTransition(&transitionno_timeout,WATER_SENSORS_READ);  // 
  //MQTT_STATE_STANDBY->addTransition(&transitiontimeout_water_sensor,MQTT_SEND);
  //MQTT_STATE_STANDBY->addTransition(&transitiontimeout_temp_hum,MQTT_SEND);
  //MQTT_STATE_STANDBY->addTransition(&transitiontimeout_lum_waterlevel,MQTT_SEND);
  MQTT_SEND->addTransition(&transitionmsg_sent,WATER_SENSORS_READ);
  WATER_SENSORS_READ->addTransition(&transitionwater_sensor_event_readed,ENVIROMENT_SENSOR_READ);  // S
  ENVIROMENT_SENSOR_READ->addTransition(&transitionenviroment_sensor_event_readed, CHECK_CONNECTION);  // 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  machine.run();
  delay(STATE_DELAY);
}
/////////////////// estados/////////////////////////////////
void stateCHECK_CONNECTION(){
  //Serial.println("State Check_conn");
  wifi_esp32.connect_live();
   
}

void stateMQTT_STATE_STANDBY(){
  //Serial.println("State mqtt stanby");
  MQTT_STATE_STANDBY->setTransition(0,3);

  if(millis()-previous_time_water> sampling_water)
  {
      //Serial.println("time out water");  
      previous_time_water=millis();
      timeout_water_sensor=1;
      MQTT_STATE_STANDBY->setTransition(0,2);
  }

   if(millis()-previous_time_lum> sampling_lum)
  {
    //Serial.println("time out lum"); 
    previous_time_lum=millis();
    timeout_lum_waterlevel=1;
    MQTT_STATE_STANDBY->setTransition(0,2);
  }

   if(millis()-previous_time_temp_hum> sampling_temp_hum)
  {
    //Serial.println("time out temphum"); 
    previous_time_temp_hum=millis();
    timeout_temp_hum=1; 
    MQTT_STATE_STANDBY->setTransition(0,2); 
  }
  
}

void stateMQTT_SEND(){
  //Serial.println("State mqtt sent");
  
  if(timeout_lum_waterlevel==1)
  {
    //envia mqtt
    //Serial.println("envio lum");
    timeout_lum_waterlevel=0;
  }
  if(timeout_temp_hum==1)
  {
    //envia mqtt
    //Serial.println("envio temp");
    timeout_temp_hum=0;
  }
  if(timeout_water_sensor==1)
  {
    //envia mqtt
    //Serial.println("envio water");
    timeout_water_sensor=0;
  }


}



void stateWATER_SENSORS_READ()
{
  
  ///
  //task
  
}

void stateENVIROMENT_SENSOR_READ(){
  //
  //task
}



bool transitionconn_alive()
{
return true;
}

bool transitionno_timeout()
{
return true;
}

bool transitionwater_sensor_event_readed()
{
return true;
}

bool transitionenviroment_sensor_event_readed()
{
return true;
}

bool transitionmsg_sent()
{
  return true;
}

