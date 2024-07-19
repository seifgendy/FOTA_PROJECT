#include "mqtt.h"

/*********************************** Define MQTT Objects *********************************************/

// Create an ESP32 WiFiClient class to connect to the MQTT server.
 static WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// MQTT Feeds

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
 Adafruit_MQTT_Publish LED_Status = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/led-status");

// Setup a feed called 'BootloaderCommand' for subscribing to changes.
 Adafruit_MQTT_Subscribe LED_Control = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led-control");

 /*********************************** Global Functions Definition *********************************************/
 void MQTT_Connect()
 {
    int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
 }