#include <Control_SM.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>


const size_t capacity = 512;
const int eepromsize = 512;
DynamicJsonDocument doc(capacity);

State ControlState = Standby;
String newTable= "{\"events\":[{\"type\":\"light\",\"start_time\":\"20:00:00\",\"end_time\":\"20:10:00\",\"on_time\":\"00:05:00\",\"off_time\":\"00:05:00\"},{\"type\":\"water\",\"start_time\":\"06:00:00\",\"end_time\":\"18:00:00\",\"on_time\":\"00:05:00\",\"off_time\":\"00:05:00\"}]}";
String currentTime ="";
String table= "{\"events\":[{\"type\":\"light\",\"start_time\":\"20:00:00\",\"end_time\":\"20:10:00\",\"on_time\":\"00:05:00\",\"off_time\":\"00:05:00\"},{\"type\":\"water\",\"start_time\":\"06:00:00\",\"end_time\":\"18:00:00\",\"on_time\":\"00:05:00\",\"off_time\":\"00:05:00\"}]}";;




void ControlTasks() {
  Serial.print(1);
  switch (ControlState) {
    case Standby:
        Serial.print(F("sby state "));

      if (newTableReceived()) {
        ControlState = Store;
      } else {
        delay(6000);
        ControlState = Execute;
      }
      break;
    case Execute:
    Serial.print(F("execute state "));
      table = retrieveTableFromEEPROM();
      currentTime = getCurrentTime();
      executeEvents(table, currentTime);
      ControlState = Standby;
      break;
    case Store:
        Serial.print(F("store state "));

      storeTableInEEPROM(newTable);
      ControlState = Standby;
      break;
  }
}

void executeEvents(String table, String currentTime) {
  // Convert the message string into a JsonDocument
  DeserializationError error = deserializeJson(doc, table);

  // Check for errors
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // Get the events array from the document
  JsonArray events = doc["events"];

  // Loop through the events in the array
  for (JsonVariant event : events) {
    // Get the event type
    const char* event_type = event["type"];
    // Get the start time
    const char* start_time = event["start_time"];
    // Get the duration
    int duration = event["duration"];

    // Check if the event has a duty_cycle field
    double duty_cycle = event["duty_cycle"].as<double>();

    // Perform some action based on the event type and other fields
    if (strcmp(event_type, "light") == 0) {
      // Check if the current time is within the start time and duration of the event
      if (currentTime >= start_time && currentTime < start_time + duration) {
        // Turn the lights on
            Serial.print("light on ");

      } else {
        // Turn the lights off
            Serial.print("light off ");

      }
    } else if (strcmp(event_type, "water") == 0) {
            if (currentTime >= start_time && currentTime < start_time + duration) {
        // Turn the lights on
            Serial.print("water on ");

      } else {
        // Turn the lights off
            Serial.print("water off ");

      }
    }
  }
}


bool newTableReceived() {
  // Replace this with the actual implementation to check if a new table has been received
  return table != newTable;
}


String retrieveTableFromEEPROM() {
  // Allocate memory for the EEPROM data
    char myTable[320];
    EEPROM.get(0,myTable);
    return myTable;
}

String getCurrentTime() {
  // Set NTP server
  const char* ntpServer = "pt.pool.ntp.org";

  // Set timezone
  const long gmtOffset_sec = 3600;
  const int daylightOffset_sec = 3600;

  // Create UDP object
  WiFiUDP udp;

  // Initialize NTP client
  NTPClient timeClient(udp,ntpServer, gmtOffset_sec, daylightOffset_sec);

  // Set interval to update time (every hour)
  timeClient.setUpdateInterval(3600);

  // Update time
  timeClient.update();

  // Get current time
  String currentTime = timeClient.getFormattedTime();

  Serial.println("Current time: " + currentTime);


  return currentTime;
}

void storeTableInEEPROM(const String& table) {
  // Allocate memory for the EEPROM data
   char myTable[320];

  strcpy(myTable,table.c_str());
  EEPROM.put(0,myTable);

}
