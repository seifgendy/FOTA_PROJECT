#include <Arduino.h>

// put function declarations here:


void setup() 
{
 pinMode(2,OUTPUT);
}
void loop() 
{
 digitalWrite(2,HIGH);
 delay(1000);
 digitalWrite(2,LOW);
 delay(1000);
  // put your main code here, to run repeatedly:
}

