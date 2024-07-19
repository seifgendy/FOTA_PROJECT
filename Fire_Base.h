#ifndef FIRE_BASE_H
#define FIRE_BASE_H

/****************** Includes*********************************/
#include <Firebase_ESP_Client.h>
#include <LittleFS.h>

/****************** Macros *********************************/

#define DOWNLOAD_SUCCESSFULL 0x01
#define DOWNLOAD_FAILED 0x00
/* 1. Define the API Key */
#define API_KEY "AIzaSyB7XWykv9oUR8xQN0vF2bc-K42wQ_2Uxuw"

/* 2. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "fota.course25@gmail.com"
#define USER_PASSWORD "12345678"

/* 3. Define the Firebase storage bucket ID e.g bucket-name.appspot.com */
#define STORAGE_BUCKET_ID "fota-project-36c53.appspot.com"



/****************** Global Functions Prototypes *********************************/
void FireBase_Init();

uint8_t FireBase_DownloadFile(std::string server_file , std::string local_file);

void fcsDownloadCallback(FCS_DownloadStatusInfo info);



#endif

