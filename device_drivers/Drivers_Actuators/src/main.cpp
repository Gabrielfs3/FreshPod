#include "Arduino.h"
#include "Actuators.h"


void setup() 
{
    Serial.begin(115200);
    Init_AllActuators();
}

void loop() 
{
   
    boolean a = Return_LED();
    boolean b = Return_Fan();
    boolean c = Return_Pump();
   
    Serial.println(a);
    Serial.println(b);
    Serial.println(c); 

    a = a^1;
    b = b^1;
    c = c^1;
    setPump(a);
    setLED(a);
    setFan(a);

     delay(1000);

 
}
