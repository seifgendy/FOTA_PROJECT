#include <Arduino.h>
#include "Conncect_to_wifi.h"

#include "BL_Host.h"
void BL_Command_Handler(uint32_t data)
{
  memset(BL_Host_Tx_Buffer,0,BL_HOST_BUFFER_TX_LENGTH);
  switch(data){
    case CBL_GET_VER_CMD :
    Serial.println("\nRequest the bootloader version");
    BL_Host_Get_Version();
    break;
    case CBL_GET_CID_CMD:
    Serial.println("\nRead the MCU chip identification number");
    BL_Host_Get_Chip_ID();
    break;
    case CBL_GET_RDP_STATUS_CMD:
    Serial.println("\nRead the FLASH Read Protection level");
    BL_Host_Get_RDP();
    break;
    case CBL_MEM_WRITE_CMD:
    Serial.println("\nFlash Application command");
    BL_Host_Flash_Application();
    break;
    case CBL_FLASH_ERASE_CMD:
    Serial.println("\nErase Applcation command");
    BL_Host_Erase_Application();
    break;
    case CBL_GO_TO_MAIN_APP_CMD:
    Serial.println("\nJump to Main Application command");
    BL_Host_Jump_To_Application();
    break; 
  }
}
void setup() 
{
  Serial.begin(230400);
  Serial2.begin(230400, SERIAL_8N1, 16, 17);
  Connect_Wifi(SSID,PASS);
  mqtt.subscribe(&BL_Command);
  BL_Command.setCallback(BL_Command_Handler);
  FireBase_Init();
  
}
void loop() 
{
  MQTT_Connect();
  mqtt.processPackets(5000);
  // put your main code here, to run repeatedly:
}
