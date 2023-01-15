#include <Arduino.h>
#include "Alarmistic_SM.h"
#include "PowerMeter.h"
#include "Actuators.h"

#include <WiFi.h>
#include <PubSubClient.h>
#include "WifiEsp32.h"

#define UPDATE_TIME                     5000
#define Task_TIME                       2000 

//estados
enum struct trafStatesALARMISTIC : int        
{
  STAND_BY_ALARMISTICA, 
  READ_AND_CHECK,
  TROBLESHOOTING,
  SEND_ALARM
};

//Eventos
boolean read_equal_theo;                 
boolean timeout_consumption_data;
boolean read_not_equal_theo;
boolean false_alarm;
boolean Alarm_2_Send;
boolean Alarm_Sent;

//definir clases
PowerMeter  power_meter(CF_PIN,CF1_PIN,SEL_PIN,CURRENT_MODE,relay_PIN);

//WiFiClient net;
//PubSubClient client(net);

//global variables
int lastSwitchTime = 0, currentstate =0;
int lastSwitchTimeCheck=0;

extern int geral_power;
const char* outTopicAlarm = "/ALarm";

//definir funções
bool troubleshooting(), check();
void power_task();


void  InitAlarmistic()
{
  Serial.begin(115200);

  power_meter.Init();
  Init_AllActuators();
}

//initial state
trafStatesALARMISTIC state = trafStatesALARMISTIC::STAND_BY_ALARMISTICA; 

int runSwitchCase(int timeMs)        //state machine
{

    if(timeout_consumption_data==1)
    {
      state = trafStatesALARMISTIC::READ_AND_CHECK;
    }

    if(read_equal_theo==1)
    {
      state = trafStatesALARMISTIC::STAND_BY_ALARMISTICA;
    }
    
    if(read_not_equal_theo==1)
    {
      state = trafStatesALARMISTIC::TROBLESHOOTING;
    }
    
    if(false_alarm==1)
    {
      state = trafStatesALARMISTIC::STAND_BY_ALARMISTICA;
    }

    if(Alarm_2_Send==1)
    {
      state = trafStatesALARMISTIC::SEND_ALARM;
    }
    
    if(Alarm_Sent==1)
    {
      state = trafStatesALARMISTIC::STAND_BY_ALARMISTICA;
    }

    switch (state)
    {
    case trafStatesALARMISTIC::STAND_BY_ALARMISTICA: 

      if((timeMs-lastSwitchTime)> UPDATE_TIME)
          {
            lastSwitchTime = timeMs;
            timeout_consumption_data=1;
            read_equal_theo=0;                 
            read_not_equal_theo=0;
            false_alarm=0;
            Alarm_2_Send=0;
            Alarm_Sent=0;
            currentstate =0;
            Serial.println("ESTADO  0 "); 
          }
        break;
    case trafStatesALARMISTIC::READ_AND_CHECK:  

      //verificar o valor teorico
      if (check()) 
      {
        read_equal_theo=1;                 
        read_not_equal_theo=0;
      }
      else
      {
        read_equal_theo=0;                 
        read_not_equal_theo=1;
      }
      timeout_consumption_data=0;
      false_alarm=0;
      Alarm_2_Send=0;
      Alarm_Sent=0;
      
      currentstate =1;
      Serial.println("ESTADO  1 "); 
      break;

    
    case trafStatesALARMISTIC::TROBLESHOOTING: 
        //chamar uma função bool troubleshooting()

        if( troubleshooting())
        {
          Serial.println("enviar alarm"); //debug
          false_alarm=0;
          Alarm_2_Send=1;
        } 
        else
        { 
          Serial.println("falso alarm");  //debug
          false_alarm=1;
          Alarm_2_Send=0;
        } 

      timeout_consumption_data=0;
      read_equal_theo=0;                 
      read_not_equal_theo=0;
      Alarm_Sent=0;

      currentstate =2;
      Serial.println("ESTADO  2 ");
      break;

    case trafStatesALARMISTIC::SEND_ALARM: 
        //MANDAR UMA MENSAGEM PARA O GRAFANA
        //sprintf(message_ALARM, "")
        //client.publish(outTopicAlarm,message_ALARM);

        timeout_consumption_data=0;
        read_equal_theo=0;                 
        read_not_equal_theo=0;
        false_alarm=0;
        Alarm_2_Send=0;
        Alarm_Sent=1;
        currentstate =3;
        Serial.println("ESTADO  3 ");
        break;
    }

return currentstate;
}

void AlarmisticTasks()
{
  int timeMs = millis();
  currentstate = runSwitchCase(timeMs);
}

int returnAlarmisticState(int currentstate)
{
  return currentstate;
}

bool check()
{
  float power_, power_mean;
  boolean check_power;

  power_task(); //executar a task do power meter passados o Task_TIME
  power_=power_meter.return_power(); //Potencia instantanea

  if (geral_power==power_)
  {
    check_power=1; 
  }
  else
  {
    check_power=0;
  }
  
  return check_power; 
}

bool troubleshooting()
{
  //variables
  boolean alarm_Pump=1, alarm_LEDS=1, alarm=1;
  float Wh_Pump_off=0,Wh_Pump_on,Wh_LEDS_off,Wh_LEDS_on;

  //---[watter pump] ----
  setPump(0); //Turn off=> 0
  power_task();
  Wh_Pump_off=power_meter.return_energy();
  Serial.println(Wh_Pump_off); //debug
  setPump(1);//Turn on => 1
  power_task();
  Wh_Pump_on=power_meter.return_energy();
  Serial.println(Wh_Pump_off); //debug

  if(Wh_Pump_on > Wh_Pump_off)
  {
    alarm_Pump=0;
  }

  //---[LEDS]----
  setLED(0);//Turn off=> 0
  power_task();
  Wh_LEDS_off=power_meter.return_energy();
  setLED(1);//Turn on=> 1
  power_task();
  Wh_LEDS_on=power_meter.return_energy();

  if(Wh_LEDS_on > Wh_LEDS_off)
  {
    alarm_LEDS=0;
  }

  //-------------------
  if ((alarm_LEDS==0) && (alarm_Pump==0))
  {
    alarm=0;
  }

  return alarm;
}

void power_task()
{
  int CheckTimeMs=0, CurrentMilis=millis(),lastSwitchTimeTask=0;
  CheckTimeMs=CurrentMilis;
  
  while (lastSwitchTimeTask!= CheckTimeMs)
  {
    CheckTimeMs = millis(); //valor atual dos milisec
    //Serial.println("Dentro do WHILE");
    if((CheckTimeMs-lastSwitchTimeTask)>(CurrentMilis+Task_TIME))
    {
      lastSwitchTimeTask = CheckTimeMs;
      Serial.println("Dentro do IF-> task powermeter");
      power_meter.power_task(CurrentMilis+Task_TIME);
    }
  }
}