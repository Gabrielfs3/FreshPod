#include <Arduino.h>
#include <LinkedList.h>
#include <StateMachine.h>
#include <Wifi_esp32.h>
#include <MQTT.h>

const char* rede = "A51";
const char* password = "qwerty123";


Wifi_esp32 wifi_esp32(rede,password);
MQTTClient client;


const int STATE_DELAY = 1000;
int randomState = 0;
const int LED1 = 12;
const int LED2 = 13;
const int LED3 = 14;

StateMachine machine = StateMachine();
void stateCheck_conn();
void stateWater_sensors();
void stateEnviroment_sensors();
void stateSend_mqtt();

bool transitionMqttWifi_off();
bool transitionRead_water_sensors();
bool transitionRead_enviroment_sensors();
bool transitionRequest_msg();
bool transitionFail_msg();
bool transitionSend_msg();



State* S0 = machine.addState(&stateCheck_conn); 
State* S1 = machine.addState(&stateWater_sensors);
State* S2 = machine.addState(&stateEnviroment_sensors);
State* S3 = machine.addState(&stateSend_mqtt);



void setup() {
  wifi_esp32.connect();
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);


  S0->addTransition(&transitionMqttWifi_off,S0);    // Transition to itself (see transition logic for details)
  S0->addTransition(&transitionRead_water_sensors,S1);  // 
  S1->addTransition(&transitionRead_enviroment_sensors,S2);  //
  
  S2->addTransition(&transitionRequest_msg,S3);  // S
  S3->addTransition(&transitionFail_msg,S0);  // 
  S3->addTransition(&transitionSend_msg,S1);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  machine.run();
  delay(STATE_DELAY);
}

void stateCheck_conn(){
  Serial.println("State Check_conn");
  wifi_esp32.connect_live();
 
      S0->setTransition(0,1);
   



  delay(10);
}
bool transitionRead_water_sensors(){
  return true;
}

bool transitionMqttWifi_off(){
  return true;
}

void stateWater_sensors(){
  Serial.println("State Water_sensor");
  digitalWrite(LED1,HIGH);
  delay(10);
}

bool transitionRead_enviroment_sensors(){
  return true;
}

//-------------------------
void stateEnviroment_sensors(){
  Serial.println("State Enviroment_sensors");
  digitalWrite(LED2,HIGH);
  delay(1000);
  
}

bool transitionRequest_msg(){
  return true;
}

//------------------------
void stateSend_mqtt(){
  Serial.println("State Send MQTT");
  digitalWrite(LED3,HIGH);
  delay(1000);
  
  S3->setTransition(0,1);
  
}


bool transitionFail_msg(){
  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED3,LOW);
  
  return true;
}

bool transitionSend_msg(){
  digitalWrite(LED2,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED3,LOW);
  return true;
}

