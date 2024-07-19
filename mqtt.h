#ifndef MQTT_H
#define MQTT_H

/****************** Includes*********************************/
#include "WiFi.h"
#include "Adafruit_MQTT_Client.h"
#include "Adafruit_MQTT.h"


/****************** Defines *********************************/

/**** Adafruit.io Setup ****/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME  "fota25"
#define AIO_KEY       "aio_mAXp100WUP765T2HR0SoaRreYCqo"

/*********************************** extern MQTT Objects *********************************************/

extern  Adafruit_MQTT_Publish LED_Status;
extern  Adafruit_MQTT_Subscribe LED_Control;
extern Adafruit_MQTT_Client mqtt;
/****************************************************************************************************/

void MQTT_Connect();

#endif