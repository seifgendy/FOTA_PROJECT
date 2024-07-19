
/*
 * Bootloader.h
 *
 *  Created on: Jul 2, 2024
 *      Author: A&G Computer World
 */

#ifndef APP_BOOTLOADER_H_
#define APP_BOOTLOADER_H_

#include <string.h>
#include "MCAL/UART/UART_Lcfg.h"
#include "MCAL/SCB/SCB_private.h"
#include "MCAL/FPEC/FPEC_interface.h"
#include "MCAL/CRC/CRC_interface.h"


#define BL_HOST_BUFFER_RX_LENGTH 200

#define BL_HOST_COMMUNICATION_UART  &UART1_Cfg


#define CBL_GET_VER_CMD              0x10
#define CBL_GET_CID_CMD              0x12
#define CBL_GET_RDP_STATUS_CMD       0x13
#define CBL_FLASH_ERASE_CMD          0x15
#define CBL_MEM_WRITE_CMD            0x16
#define CBL_GO_TO_MAIN_APP_CMD       0x18



#define CBL_VENDOR_ID 					200
#define CBL_SW_MAJOR_VERSION            1
#define CBL_SW_MINOR_VERSION            0
#define CBL_SW_PATCH_VERSION            0

#define ADDRESS_IS_INVALID           0x00
#define ADDRESS_IS_VALID             0x01


#define FLASH_BASE 	0x08000000U
#define STM32F401CCx_FLASH_END         (FLASH_BASE + STM32F401CCx_FLASH_SIZE)
#define STM32F401CCx_FLASH_SIZE        (256 * 1024)


typedef void (*pMainApp)(void);


#define JUMP_FAILED                  0x00
#define JUMP_SUCCESSFUL              0x01
#define APPLICATION_START_ADDRESS    0x08004000

#define HAL_MAX_DELAY       1800000
#define UART_MAX_DELAY       1800000

#define SUCCESSFUL_ERASE             0x03


#define APPLICATION_SECTOR_NUMBER   1
#define FLASH_NB_OF_SECTORS      	6


#define IDCODE_REG 			(*((volatile u32*)0xE0042000))

void  BL_UART_Fetch_Host_Command(void);

#define CRC_VERIFICATION_FAILED       0
#define CRC_VERIFICATION_PASSED       1


#define CBL_SEND_NACK                0xAB
#define CBL_SEND_ACK                 0xCD


#endif /* APP_BOOTLOADER_H_ */
