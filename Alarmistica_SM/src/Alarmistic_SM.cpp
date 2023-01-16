#include <Arduino.h>
#include "Alarmistic_SM.h"
#include "PowerMeter.h"
#include "Actuators.h"

#include <WiFi.h>
#include <PubSubClient.h>
#include "WifiEsp32.h"

#define UPDATE_TIME                     5000
#define Task_TIME                       5000 


#define all_ON  40
#define LEDsON_PumpOFF 40
#define LEDsOFF_PumpON 40
#define LEDsOFF_PumpOFF 2
#define Erro 4 

//estados
enum struct trafStatesALARMISTIC : int        
{
  STAND_BY_ALARMISTICA, 
  READ_AND_CHECK,
  TROBLESHOOTING,
  SEND_ALARM
};

//Eventos
boolean read_equal_theo=0;                 
boolean timeout_consumption_data=0;
boolean read_not_equal_theo=0;
boolean false_alarm=0;
boolean Alarm_2_Send=0;
boolean Alarm_Sent=0;

//definir clases
PowerMeter  power_meter(CF_PIN,CF1_PIN,SEL_PIN,CURRENT_MODE,relay_PIN);

WiFiClient net;
PubSubClient client(net);

//global variables
int lastSwitchTime = 0, currentstate =0;
int lastSwitchTimeCheck=0;

boolean LastStateLED;
boolean LastStatePump;

extern int geral_power;
const char* outTopicAlarm = "/ALarm";
char message_Alarm_buffer[2];

//definir funções
bool troubleshooting(), check();


void  InitAlarmistic()
{
  Serial.begin(115200);

  power_meter.Init();
  Init_AllActuators();
  setLED(1);
  setPump(0);
}

//initial state
trafStatesALARMISTIC state = trafStatesALARMISTIC::STAND_BY_ALARMISTICA; 

int runSwitchCase(int timeMs)        //state machine
{
  
    if(timeout_consumption_data==1)
    {
      state = trafStatesALARMISTIC::READ_AND_CHECK;
      timeout_consumption_data=0;
    }

    if(read_equal_theo==1)
    {
      state = trafStatesALARMISTIC::STAND_BY_ALARMISTICA;
      read_equal_theo=0;
    }
    
    if(read_not_equal_theo==1)
    {
      state = trafStatesALARMISTIC::TROBLESHOOTING;
      read_not_equal_theo=0;
    }
    
    if(false_alarm==1)
    {
      state = trafStatesALARMISTIC::STAND_BY_ALARMISTICA;
      false_alarm=0;
    }

    if(Alarm_2_Send==1)
    {
      state = trafStatesALARMISTIC::SEND_ALARM;
      Alarm_2_Send=0;
    }
    
    if(Alarm_Sent==1)
    {
      state = trafStatesALARMISTIC::STAND_BY_ALARMISTICA;
      Alarm_Sent=0;
    }

    switch (state)
    {
    case trafStatesALARMISTIC::STAND_BY_ALARMISTICA: 

      if((timeMs-lastSwitchTime)> UPDATE_TIME)
          {
            lastSwitchTime = timeMs;
            timeout_consumption_data=1;
            currentstate =0;
            Serial.println("ESTADO  StandBY "); 
          }
        break;
    case trafStatesALARMISTIC::READ_AND_CHECK:  

      //verificar o valor teorico
      //Serial.println(!check());
      if (!check()) 
      {
        read_equal_theo=1;                   
      }
      else
      {                 
        read_not_equal_theo=1;
      } 
      currentstate =1;
      Serial.println("ESTADO  Check "); 
      break;

    
    case trafStatesALARMISTIC::TROBLESHOOTING: 
        //chamar uma função bool troubleshooting()
           LastStateLED=Return_LED();
           LastStatePump=Return_Pump();

        if( troubleshooting())
        {
          Serial.println("enviar alarm"); //debug
          Alarm_2_Send=1;
        } 
        else
        { 
          Serial.println("falso alarm");  //debug
          false_alarm=1;
        } 
         setLED(LastStateLED);
         setPump(LastStatePump);
      currentstate =2;
      Serial.println("ESTADO  Troble ");
      break;

    case trafStatesALARMISTIC::SEND_ALARM: 
        //MANDAR UMA MENSAGEM PARA O GRAFANA
        sprintf(message_Alarm_buffer, "S");
        client.publish(outTopicAlarm,message_Alarm_buffer);

    
        Alarm_Sent=1;
        currentstate =3;
        Serial.println("ESTADO  Send_alarm ");
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
  float power_instant =power_meter.instant_power();
  boolean check_power;
Serial.println(power_instant);
  if((Return_LED()==1) && (Return_Pump()==1))
  {
    Serial.println("onon");
    if((power_instant> (all_ON - Erro)) && (power_instant< all_ON + Erro) )
        {
          check_power=0;
        }
    else
      {
         check_power=1; 
      }
  }

  else if((Return_LED()==0) && (Return_Pump()==1))
  {
    Serial.println("offon");
    if((power_instant> LEDsOFF_PumpON - Erro) && (power_instant< LEDsOFF_PumpON + Erro) )
        {
          check_power=0;
        }
    else
      {
         check_power=1; 
      }
  }

    else if((Return_LED()==1) && (Return_Pump()==0))
     {
    Serial.println("onoff");
    if((power_instant> LEDsON_PumpOFF - Erro) && (power_instant< LEDsON_PumpOFF + Erro) )
        {
          check_power=0;
        }
    else
      {
         check_power=1; 
      }
  }

    else if((Return_LED()==0) && (Return_Pump()==0))
  {
    Serial.println("offoff");
    if((power_instant> LEDsOFF_PumpOFF - Erro) && (power_instant< LEDsOFF_PumpOFF + Erro) )
        {
          check_power=0;
        }
    else
      {
         check_power=1; 
      }
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
  Wh_Pump_off=power_meter.instant_power(); //potencia instantanea
  Serial.print("Pump: ");
  Serial.print(Wh_Pump_off); //debug
  setPump(1);//Turn on => 1
  Wh_Pump_on=power_meter.instant_power();
  Serial.println(Wh_Pump_on); //debug

  if((Wh_Pump_on-Wh_Pump_off > 1)) //ter uma gama (Wh_Pump_on-Wh_Pump_off >gama)
  {
    alarm_Pump=0;
  }

    //---[LEDS]----
  setLED(0);//Turn off=> 0
  Wh_LEDS_off=power_meter.instant_power(); //potencia instantanea
  Serial.print("LEDS: ");
  Serial.print(Wh_LEDS_off);
  setLED(1);//Turn on=> 1
  Wh_LEDS_on=power_meter.instant_power();
  Serial.println(Wh_LEDS_on);

  if((Wh_LEDS_on-Wh_LEDS_off > 1)) //ter uma gama
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