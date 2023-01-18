#include <testeeeprom.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>

//Constants
   int EEPROM_SIZE = EEPROM.length();
//char myString[1024];
String json = "{\"events\":[{\"type\":\"light\",\"start_time\":\"20:00:00\",\"end_time\":\"20:10:00\",\"on_time\":\"00:05:00\",\"off_time\":\"00:05:00\"},{\"type\":\"water\",\"start_time\":\"06:00:00\",\"end_time\":\"18:00:00\",\"on_time\":\"00:05:00\",\"off_time\":\"00:05:00\"}]}";
String myString = "ola";

void testeprom()
{  // Init Serial USB
  Serial.begin(115200);
   char myBoard[320];

  strcpy(myBoard,json.c_str());
  EEPROM.put(0,myBoard);
    
    char theBoard[320];
    EEPROM.get(0,theBoard);
    Serial.println(theBoard);




/*

// Calculate the required size for the buffer
size_t bufferSize = json.length() + 1;

// Allocate a buffer on the heap
strcpy(myString,json.c_str());

Serial.println(myString);

// Write the buffer to the EEPROM
EEPROM.begin(bufferSize);
EEPROM.put(0, myString);
EEPROM.end();

// Free the buffer from the heap
char newbuffer[1024];

// Read the buffer from the EEPROM
EEPROM.begin(bufferSize);
EEPROM.get(0, newbuffer);
EEPROM.end();

// Convert the buffer to a String object
String json(newbuffer);
Serial.println(json);

DynamicJsonDocument doc(1024);
deserializeJson(doc, json);

String prettyJson;
serializeJsonPretty(doc, prettyJson);

Serial.println(prettyJson);

*/
}