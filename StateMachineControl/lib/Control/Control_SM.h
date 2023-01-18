#ifndef Control_SM
#define Control_SM

#include <Arduino.h>
#include <ArduinoJson.h>

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
void storeTableInEEPROM(const String& table);

#endif