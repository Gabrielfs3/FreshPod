#ifndef Control_SM
#define Control_SM

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

enum State {
  Standby,
  Execute,
  Store,
};

void ControlTasks();
void executeEvents(String table, String currentTime);
bool newTableReceived();
String retrieveTableFromEEPROM();
String getCurrentTime();
void storeTableInEEPROM( String& table);
String getnewtable();
bool isTimeToActivate(String currentTime, String startTime, String onMinute, String offMinute, String endTime);
void clearEEPROM();
void printEEPROM();
String getCurrentTime(WiFiUDP &udp, NTPClient &timeClient);

#endif