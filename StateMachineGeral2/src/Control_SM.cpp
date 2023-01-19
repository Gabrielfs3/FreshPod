#include <Telemetry_SM2.h>
#include <Control_SM.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <MQTT.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include <Actuators.h>

const size_t capacity = 2048;
const int eepromsize = 2048;
DynamicJsonDocument doc(capacity);

State ControlState = Standby;
String currentTime ="";
String ControlTopic = "/Actuadores/Tabela/Control";

String table= "";
String TableToStore="";
String tabletoeeprom="";

WiFiUDP udp;

// Create NTPClient object
const char* ntpServer = "pt.pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;
NTPClient timeClient2(udp, ntpServer, gmtOffset_sec, daylightOffset_sec);


void ControlTasks() {
  Serial.print(1);
  switch (ControlState) {
    case Standby:
      Serial.print(F("sby state "));

      if (newTableReceived()) {
        tabletoeeprom="";
        table = TableToStore;
        clearEEPROM();
        printEEPROM();
        ControlState = Store;
      } 
      else 
      {
        delay(1000);
        ControlState = Execute;
      }
      break;
    case Execute:
    Serial.print(F("execute state "));
      tabletoeeprom=retrieveTableFromEEPROM();
      currentTime = getCurrentTime(udp, timeClient2);
      executeEvents(tabletoeeprom, currentTime);
      ControlState = Standby;
      break;
    case Store:
      Serial.print(F("store state "));
      clearEEPROM();
      storeTableInEEPROM(table);
      ControlState = Standby;
      break;
  }
}


bool newTableReceived() {
  if(Controlloop()!=TableToStore)
  {
    TableToStore = "";
    TableToStore = Controlloop();
  }

  return table != TableToStore;
}

String retrieveTableFromEEPROM() {
  // Allocate memory for the EEPROM data
    char tableouteeprom[2048];
    EEPROM.get(0,tableouteeprom);
    return tableouteeprom;
}

void storeTableInEEPROM(String& tablein) {
  // Allocate memory for the EEPROM data

  char myTable[2048];
  strcpy(myTable,tablein.c_str());
 
  EEPROM.put(0,myTable);
 
  EEPROM.commit();
}

void clearEEPROM() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.put(i,0);
  }
    EEPROM.commit();

}

String getCurrentTime(WiFiUDP &udp, NTPClient &timeClient2) {
  
    // Set timezone
    const long gmtOffset_sec = 0;
    const int daylightOffset_sec = 0;
    
    // Update time
    timeClient2.update();


    // Get current time
    String currentTime = timeClient2.getFormattedTime();

    Serial.println("Current time: " + currentTime);
    return currentTime;
}



void executeEvents(String tabletoexecute,String currentTimetocompare)
{

  //tabletoexecute = "[{\"_id\": {\"$oid\": \"63c3e30507d5b101bcf6d196\"}, \"timestamp\": 1673782021325.0, \"Leds\": {\"stime\": \"12:08\", \"ontime\": \"01\", \"offtime\": \"01\", \"endtime\": \"12:25\"}, \"Fans\": {\"stime\": \"x\", \"ontime\": \"x\", \"offtime\": \"x\", \"endtime\": \"x\"}, \"Water Pump\": {\"stime\": \"12:04\", \"ontime\": \"1\", \"offtime\": \"1\", \"endtime\": \"12:10\"}, \"_msgid\": \"81eb714d77b878de\"}]";
  //tabletoexecute.remove(0,1);
  //tabletoexecute.remove(tabletoexecute.length());
  
  tabletoexecute.remove(0,2); // remove the first "
  tabletoexecute.remove(tabletoexecute.length()-1,2); // remove the last "
  tabletoexecute.replace("\\","");
  

  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, tabletoexecute);

  if (error) {
        Serial.println("Failed to parse JSON string inside execute");
  }
    // extract data
    String _id = doc["_id"]["$oid"];
    float timestamp = doc["timestamp"];
    String led_stime = doc["Leds"]["stime"];
    String led_ontime = doc["Leds"]["ontime"];
    String led_offtime = doc["Leds"]["offtime"];
    String led_endtime = doc["Leds"]["endtime"];
    String fan_stime = doc["Fans"]["stime"];
    String fan_ontime = doc["Fans"]["ontime"];
    String fan_offtime = doc["Fans"]["offtime"];
    String fan_endtime = doc["Fans"]["endtime"];
    String pump_stime = doc["Water Pump"]["stime"];
    String pump_ontime = doc["Water Pump"]["ontime"];
    String pump_offtime = doc["Water Pump"]["offtime"];
    String pump_endtime = doc["Water Pump"]["endtime"];
    String _msgid = doc["_msgid"];

      
    Serial.println("_id: " + _id);
    Serial.println("timestamp: " + String(timestamp));
    Serial.println("led_stime: " + led_stime);
    Serial.println("led_ontime: " + led_ontime);
    Serial.println("led_offtime: " + led_offtime);
    Serial.println("led_endtime: " + led_endtime);
    Serial.println("fan_stime: " + fan_stime);
    Serial.println("fan_ontime: " + fan_ontime);
    Serial.println("fan_offtime: " + fan_offtime);
    Serial.println("fan_endtime: " + fan_endtime);
    Serial.println("pump_stime: " + pump_stime);
    Serial.println("pump_ontime: " + pump_ontime);
    Serial.println("pump_offtime: " + pump_offtime);
    Serial.println("pump_endtime: " + pump_endtime);
    Serial.println("_msgid: " + _msgid);

      if (isTimeToActivate(currentTimetocompare,led_stime, led_ontime, led_offtime, led_endtime)) 
      {
        Serial.println("leds on ");
        setLED(1);
      } 
      else 
      {
        Serial.println("leds off");
        setLED(0);
      }

      if (isTimeToActivate(currentTimetocompare,pump_stime, pump_ontime, pump_offtime, pump_endtime)) 
      {
        Serial.println("Pump on ");
        setPump(1);
      } 
      else 
      {
        Serial.println("Pump off");
        setPump(0);
      }
   
}

bool isTimeToActivate(String currentTime, String startTime, String onMinute, String offMinute, String endTime) {
    int onMinutes = onMinute.toInt();
    int offMinutes = offMinute.toInt();
    int currentHour = currentTime.substring(0,2).toInt();
    int currentMinutes = currentTime.substring(3, 5).toInt();
    int startHour = startTime.substring(0, 2).toInt();
    int startMinutes = startTime.substring(3, 5).toInt();
    int endHour = endTime.substring(0, 2).toInt();
    int endMinutes = endTime.substring(3, 5).toInt();
    int currentTotalMinutes = (currentHour * 60) + currentMinutes;
    int startTotalMinutes = (startHour * 60) + startMinutes;
    int endTotalMinutes = (endHour * 60) + endMinutes;
    int timeSinceStart = currentTotalMinutes - startTotalMinutes;
    int cycleCount = timeSinceStart / (onMinutes + offMinutes);
    int currentCycleStart = cycleCount * (onMinutes + offMinutes);
    int currentCycleEnd = currentCycleStart + onMinutes;
    if(currentTotalMinutes >= startTotalMinutes && currentTotalMinutes <= endTotalMinutes && (timeSinceStart >= currentCycleStart && timeSinceStart < currentCycleEnd)) {
        return true;
    }
    else{
        return false;
    }
}


void printEEPROM() {
  for (int i = 0; i < EEPROM.length(); i++) {
    Serial.print((char)EEPROM.read(i));
  }
  Serial.println();
}