#include <Arduino.h>
#include "Conncect_to_wifi.h"
#include "mqtt.h"

void LED_Control_callback(uint32_t data)
{
  switch (data)
  {
  case 1:
    digitalWrite(2,HIGH);
    LED_Status.publish("LED IS ON");
    break;
  case 0:
  digitalWrite(2,LOW);
  LED_Status.publish("LED IS OFF");
    break;
  default:
    break;
  }

}
void setup() 
{
  Serial.begin(230400);
  pinMode(2,OUTPUT);
  Connect_Wifi(SSID,PASS);
  mqtt.subscribe(&LED_Control);
  LED_Control.setCallback(LED_Control_callback);
  
  
}
void loop() 
{
MQTT_Connect();
mqtt.processPackets(5000); 

  // put your main code here, to run repeatedly:
}
