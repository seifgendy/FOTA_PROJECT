/*
 * Bootloader.c
 *
 *  Created on: Jul 2, 2024
 *      Author: A&G Computer World
 */


#include "Bootloader.h"



static u8 BL_HOST_RX_BUFFER[200]  ;





static void Bootloader_Get_Version(u8 * BL_HOST_BUFFER);
static void Bootloader_Get_Chip_ID(u8 * BL_HOST_BUFFER);
static void Bootloader_Get_RDP_Status(u8 *BL_HOST_BUFFER);
static void Bootloader_Application_Erase(u8 *BL_HOST_BUFFER);
static void Bootloader_Upload_Application(u8 *BL_HOST_BUFFER);
static void Bootloader_Jump_To_Application(u8 *BL_HOST_BUFFER);
static void Bootloader_Send_ACK(u8 Datalen);
static void Bootloader_Send_NACK();
static void Bootloader_Send_Data_To_Host(u8 * Host_Buffer , u32 Host_Buffer_Len);
static u8 Host_Address_Verification(u32 HOST_Address);


static  u8 Bootloader_CRC_Verify(u8*CRC_Data,u32 CRC_Data_Size , u32 Host_CRC);

static void Bootloader_Get_Version(u8 * BL_HOST_BUFFER)
{
	u8 BL_Version[4] = { CBL_VENDOR_ID, CBL_SW_MAJOR_VERSION, CBL_SW_MINOR_VERSION, CBL_SW_PATCH_VERSION };
	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;


	Host_CMD_Packet_Len = BL_HOST_BUFFER[0]+1;
	Host_CRC32 = *((u32*)(BL_HOST_BUFFER+Host_CMD_Packet_Len-4));


	if(Bootloader_CRC_Verify(BL_HOST_BUFFER,Host_CMD_Packet_Len - 4, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
		Bootloader_Send_ACK(4);
		Bootloader_Send_Data_To_Host(BL_Version ,4);
	}
	else
	{
		Bootloader_Send_NACK();
	}
}




static void Bootloader_Get_Chip_ID(u8 * BL_HOST_BUFFER)
{

	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;
    u16 MCU_Identification_Number = 0 ;

	Host_CMD_Packet_Len = BL_HOST_BUFFER[0]+1;
	Host_CRC32 = *((u32*)(BL_HOST_BUFFER+Host_CMD_Packet_Len-4));

	MCU_Identification_Number =  IDCODE_REG & (0x00000FFF) ;

	if(Bootloader_CRC_Verify(BL_HOST_BUFFER,Host_CMD_Packet_Len - 4, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
		Bootloader_Send_ACK(2);
		Bootloader_Send_Data_To_Host(((u8*)&MCU_Identification_Number) ,2);

	}
	else
	{
		Bootloader_Send_NACK();
	}

}






static void Bootloader_Get_RDP_Status(u8 * BL_HOST_BUFFER)
{

	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;
    u8  RDP_Level = 0 ;

	Host_CMD_Packet_Len = BL_HOST_BUFFER[0]+1;
	Host_CRC32 = *((u32*)(BL_HOST_BUFFER+Host_CMD_Packet_Len-4));



	if(Bootloader_CRC_Verify(BL_HOST_BUFFER,Host_CMD_Packet_Len - 4, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
		Bootloader_Send_ACK(1);
		RDP_Level = FPEC_Get_ReadProtectionLevel();
		Bootloader_Send_Data_To_Host(&RDP_Level , 1);


	}
	else
	{
		Bootloader_Send_NACK();
	}

}



static void Bootloader_Application_Erase(u8 * BL_HOST_BUFFER)
{

	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;
    u8  Erase_Status = SUCCESSFUL_ERASE ;

	Host_CMD_Packet_Len = BL_HOST_BUFFER[0]+1;
	Host_CRC32 = *((u32*)(BL_HOST_BUFFER+Host_CMD_Packet_Len-4));



	if(Bootloader_CRC_Verify(BL_HOST_BUFFER,Host_CMD_Packet_Len - 4, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
		Bootloader_Send_ACK(1);
		FPEC_EraseSectors (APPLICATION_SECTOR_NUMBER  , FLASH_NB_OF_SECTORS - APPLICATION_SECTOR_NUMBER );
		Bootloader_Send_Data_To_Host(&Erase_Status , 1);


	}
	else
	{
		Bootloader_Send_NACK();
	}

}



static void Bootloader_Jump_To_Application(u8 * BL_HOST_BUFFER)
{

	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;

	pMainApp  ResetHandler_Address = NULL ;
	u8 Jump_Status = JUMP_FAILED ;
    u32 Temp = 0 ;
	Host_CMD_Packet_Len = BL_HOST_BUFFER[0]+1;
	Host_CRC32 = *((u32*)(BL_HOST_BUFFER+Host_CMD_Packet_Len-4));



	if(Bootloader_CRC_Verify(BL_HOST_BUFFER,Host_CMD_Packet_Len - 4, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
		Bootloader_Send_ACK(1);
        Temp  = *((volatile u32*)(APPLICATION_START_ADDRESS + 4 ) ) ;
        ResetHandler_Address = (pMainApp)Temp ;
        if(Temp == 0xFFFFFFFF)
        {

        	Jump_Status = JUMP_FAILED ;
        	Bootloader_Send_Data_To_Host(&Jump_Status , 1 );
        }

        else
        {
        	Jump_Status = JUMP_SUCCESSFUL ;
         	Bootloader_Send_Data_To_Host(&Jump_Status , 1 );
            SCB->VTOR = APPLICATION_START_ADDRESS ;
            ResetHandler_Address();
        }





	}
	else
	{
		Bootloader_Send_NACK();
	}

}











static void Bootloader_Upload_Application(u8 * BL_HOST_BUFFER)
{

	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;


	u32 HOST_Address = 0 ;
	u8 Payload_Len = 0 ;
	u8 Address_Verification = ADDRESS_IS_VALID ;

	Host_CMD_Packet_Len = BL_HOST_BUFFER[0]+1;
	Host_CRC32 = *((u32*)(BL_HOST_BUFFER+Host_CMD_Packet_Len-4));



	if(Bootloader_CRC_Verify(BL_HOST_BUFFER,Host_CMD_Packet_Len - 4, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
		Bootloader_Send_ACK(1);
		HOST_Address = *((u32*)&BL_HOST_BUFFER[2]);
		Payload_Len = BL_HOST_BUFFER[6];
		Address_Verification = Host_Address_Verification(HOST_Address);

		if(Address_Verification == ADDRESS_IS_VALID )
		{


            FPEC_Write(HOST_Address , &BL_HOST_BUFFER[7] , Payload_Len);
			Bootloader_Send_Data_To_Host(&Address_Verification , 1 );


		}
		else
		{

			Bootloader_Send_Data_To_Host(&Address_Verification , 1 );

		}

	}
	else
	{
		Bootloader_Send_NACK();
	}

}







static u8 Bootloader_CRC_Verify(u8*CRC_Data,u32 CRC_Data_Size , u32 Host_CRC)
{

	u8 CRC_Status = CRC_VERIFICATION_FAILED;
		u32 MCU_CRC_Calculated = 0;
		u8 Data_Counter = 0;
		u32 Data_Buffer = 0;


		for(Data_Counter = 0 ; Data_Counter < CRC_Data_Size ; Data_Counter++)
		{
			Data_Buffer = CRC_Data[Data_Counter];
			MCU_CRC_Calculated = CRC_Accumulate(&Data_Buffer , 1);
		}

		CRC_RESET_DR();


		if(Host_CRC == MCU_CRC_Calculated){
			CRC_Status = CRC_VERIFICATION_PASSED;
		}
		else {
			CRC_Status = CRC_VERIFICATION_FAILED;
		}



		return CRC_Status;



}





static void Bootloader_Send_ACK(u8 Datalen)
{
	u8 ACK_Arr[2] = {0};
	ACK_Arr[0] = CBL_SEND_ACK;
	ACK_Arr[1] = Datalen;
	Bootloader_Send_Data_To_Host(ACK_Arr , 2 );
}




static void Bootloader_Send_NACK(void)
{
	u8 NACK_value = CBL_SEND_NACK;
	Bootloader_Send_Data_To_Host(&NACK_value , 1);
}



static void Bootloader_Send_Data_To_Host(u8 * Host_Buffer , u32 Host_Buffer_Len){

	UART_voidTransmit(BL_HOST_COMMUNICATION_UART , Host_Buffer , Host_Buffer_Len , UART_MAX_DELAY);

}








void BL_UART_Fetch_Host_Command(void)
{
memset(BL_HOST_RX_BUFFER,0,sizeof(BL_HOST_RX_BUFFER));
UART_ReceiveByte(BL_HOST_COMMUNICATION_UART , &BL_HOST_RX_BUFFER[0] , UART_MAX_DELAY);
UART_voidReceive(BL_HOST_COMMUNICATION_UART , &BL_HOST_RX_BUFFER[1] , BL_HOST_RX_BUFFER[0] , UART_MAX_DELAY);

switch (BL_HOST_RX_BUFFER[1])
{


            case CBL_GET_VER_CMD:
            	Bootloader_Get_Version( BL_HOST_RX_BUFFER);
				break;
			case CBL_GET_CID_CMD:
			//	Bootloader_Get_Chip_ID(BL_HOST_RX_BUFFER);
				break;
					case CBL_GET_RDP_STATUS_CMD:
			//	Bootloader_Get_RDP_Status(BL_HOST_RX_BUFFER);
				break;
			case CBL_FLASH_ERASE_CMD:
			//	Bootloader_Application_Erase(BL_HOST_RX_BUFFER);
				break;
			case CBL_MEM_WRITE_CMD:
			//	Bootloader_Upload_Application(BL_HOST_RX_BUFFER);
				break;
			case CBL_GO_TO_MAIN_APP_CMD:
			//	Bootloader_Jump_To_Application(BL_HOST_RX_BUFFER);
				break;
			default:
				break;



}


}


static u8 Host_Address_Verification(u32 Host_Address)
{

	u8 Address_Verification = ADDRESS_IS_INVALID;

	if(Host_Address>= FLASH_BASE && Host_Address <= STM32F401CCx_FLASH_END )
	{
		Address_Verification = ADDRESS_IS_VALID ;


	}

	else
	{

		Address_Verification = 	 ADDRESS_IS_INVALID;

	}


	return Address_Verification ;

}
