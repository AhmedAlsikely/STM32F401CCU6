/*
 * bootloader.c
 *
 *  Created on: Jul 3, 2023
 *      Author: Alsikely
 */

/*---------------------- Section : Includes --------------------- */
#include "bootloader.h"
/*---------------------- Section : Function Declarations------------ */
static void Bootloader_Get_Version(uint8_t *Host_Buffer);
static void Bootloader_Get_Help(uint8_t *Host_Buffer);
static void Bootloader_Get_Chip_Identification_Number(uint8_t *Host_Buffer);
static void Bootloader_Read_Protection_Level(uint8_t *Host_Buffer);
static void Bootloader_Jump_To_Address(uint8_t *Host_Buffer);
static void Bootloader_Erase_Flash(uint8_t *Host_Buffer);
static void Bootloader_Memory_Write(uint8_t *Host_Buffer);
static void Bootloader_Enable_RW_Protection(uint8_t *Host_Buffer);
static void Bootloader_Memory_Read(uint8_t *Host_Buffer);
static void Bootloader_Get_Sector_Protection_Status(uint8_t *Host_Buffer);
static void Bootloader_Read_OTP(uint8_t *Host_Buffer);
static void Bootloader_Disable_RW_Protection(uint8_t *Host_Buffer);

static uint8_t Bootloader_CRC_Verify(uint8_t *pData, uint32_t Data_len, uint32_t Host_CRC);
static void Bootloader_Send_ACK(uint8_t Replay_len);
static void Bootloader_Send_NACK(void);
static void Bootloader_Send_Data_To_Host(uint8_t *Host_Buffer, uint32_t Data_len);

/*---------------------- Section : Global Variables Definitions ------- */
static uint8_t BL_Host_Buffer[BL_HOST_BUFFER_RC_LENGTH];
static uint8_t Bootloader_Support_CMDs[12] = {
		CBL_GET_VER_CMD  ,
		CBL_GET_HELP_CMD ,
		CBL_GET_CID_CMD    ,
		CBL_GET_RDP_STATUS_CMD ,
		CBL_GO_TO_ADDR_CMD ,
		CBL_FLASH_ERASE_CMD  ,
		CBL_MEM_WRITE_CMD,
		CBL_EN_R_W_PROTECT_CMD,
		CBL_MEM_READ_CMD ,
		CBL_READ_SECTOR_STATUS_CMD ,
		CBL_OTP_READ_CMD,
		CBL_DIS_R_W_PROTECT_CMD
};

/*---------------------- Section : Functions Definitions ------- */

BL_Status BL_UART_Featch_Host_Command(void){
	BL_Status Status = BL_NACK;
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint8_t Data_Length = 0;


	memset(BL_Host_Buffer, 0, BL_HOST_BUFFER_RC_LENGTH);
	HAL_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, BL_Host_Buffer, 1, HAL_MAX_DELAY);
	if(HAL_OK != HAL_Status)
	{
		Status = BL_NACK;
	}
	else
	{
		Data_Length = BL_Host_Buffer[0];
		HAL_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART,  &BL_Host_Buffer[1], Data_Length, HAL_MAX_DELAY);
		if(HAL_OK != HAL_Status)
		{
			Status = BL_NACK;
		}
		else
		{
			switch(BL_Host_Buffer[1])
			{
				case CBL_GET_VER_CMD:
					Bootloader_Get_Version(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_GET_HELP_CMD:
					//BL_print_message("Read the commands supported by the bootloader \r\n");
					Bootloader_Get_Help(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_GET_CID_CMD:

					BL_print_message("Read the MCU chip identification number \r\n");
					Bootloader_Get_Chip_Identification_Number(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_GET_RDP_STATUS_CMD:

					BL_print_message("Read the FLASH Read Protection level \r\n");
					Bootloader_Read_Protection_Level(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_GO_TO_ADDR_CMD:

					BL_print_message("Jump bootloader to specified address \r\n");
					Bootloader_Jump_To_Address(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_FLASH_ERASE_CMD:

					BL_print_message("Mass erase or sector erase of the user flash \r\n");
					Bootloader_Erase_Flash(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_MEM_WRITE_CMD:

					BL_print_message("Write data into different memories of the MCU \r\n");
					Bootloader_Memory_Write(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_EN_R_W_PROTECT_CMD:

					BL_print_message("Enable read/write protect on different sectors of the user flash \r\n");
					Bootloader_Enable_RW_Protection(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_MEM_READ_CMD:

					BL_print_message("Read data from different memories of the microcontroller \r\n");
					Bootloader_Memory_Read(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_READ_SECTOR_STATUS_CMD:

					BL_print_message("Read all the sector protection status \r\n");
					Bootloader_Get_Sector_Protection_Status(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_OTP_READ_CMD:

					BL_print_message("Read the OTP contents \r\n");
					Bootloader_Read_OTP(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_DIS_R_W_PROTECT_CMD:

					BL_print_message("Disable read/write protection on different sectors of the user flash \r\n");
					Bootloader_Disable_RW_Protection(BL_Host_Buffer);
					Status = BL_OK;
					break;
				default:
					BL_print_message("Invaild command code received from host !! \r\n");
					Status = BL_NACK;
					break;
			}
		}
	}

	return Status;
}

static uint8_t Bootloader_CRC_Verify(uint8_t *pData, uint32_t Data_len, uint32_t Host_CRC){
	uint8_t CRC_Status = CRC_VERIFICATION_FAILED;
	uint32_t MCU_CRC_Calculate = 0;
	uint8_t Data_Counter = 0;
	uint32_t Data_Buffer = 0;

	for(Data_Counter = 0; Data_Counter < Data_len; Data_Counter++){
		Data_Buffer = (uint32_t)pData[Data_Counter];
		MCU_CRC_Calculate = HAL_CRC_Accumulate(BL_CRC_ENABLE_OBJ, &Data_Buffer, 1);
	}

	__HAL_CRC_DR_RESET(BL_CRC_ENABLE_OBJ);

	if(MCU_CRC_Calculate == Host_CRC){
		CRC_Status = CRC_VERIFICATION_PASSED;
	}else{
		CRC_Status = CRC_VERIFICATION_FAILED;
	}

	return CRC_Status;
}

static void Bootloader_Send_ACK(uint8_t Replay_len){
	uint8_t ACK_Value[2]={0};
	ACK_Value[0] = CBL_SEND_ACK;
	ACK_Value[1] = Replay_len;
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)ACK_Value, 2, HAL_MAX_DELAY);
}

static void Bootloader_Send_NACK(void){
	uint8_t NACK_Value = CBL_SEND_NACK;
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, &NACK_Value, 1, HAL_MAX_DELAY);
}

static void Bootloader_Send_Data_To_Host(uint8_t *Host_Buffer, uint32_t Data_len){
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)Host_Buffer, Data_len, HAL_MAX_DELAY);
}

static void Bootloader_Get_Version(uint8_t *Host_Buffer){
	uint8_t BL_Version[4] = {CBL_VENDOR_ID, CBL_SW_MAJOR_VERSION, CBL_SW_MINOR_VERSION, CBL_SW_PATCH_VERSION};
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint8_t CRC_Verify  = 0;

#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_print_message("Read the bootloader version from the MCU \r\n");
#endif
	/* Extract the CRC32 and Packet length send by the Host */
	Host_CMD_Packet_Len = Host_Buffer[0]+1;
	Host_CRC32 =  *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_SIZE_BYTE));

	/*CRC Verification*/
	CRC_Verify = Bootloader_CRC_Verify((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_SIZE_BYTE, Host_CRC32);
	if(CRC_VERIFICATION_PASSED == CRC_Verify){
		Bootloader_Send_ACK(4);
		Bootloader_Send_Data_To_Host((uint8_t *)BL_Version, 4);
#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_print_message("CRC Verification Successful \r\n");
#endif
	}else{
		Bootloader_Send_NACK();
#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_print_message("CRC Verification failed \r\n");
#endif
	}
}


static void Bootloader_Get_Help(uint8_t *Host_Buffer){
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint8_t CRC_Verify  = 0;
#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_print_message("Read the commands supported by the bootloader \r\n");
#endif

	/* Extract the CRC32 and Packet length send by the Host */
	Host_CMD_Packet_Len = Host_Buffer[0]+1;
	Host_CRC32 =  *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_SIZE_BYTE));

	/*CRC Verification*/
	CRC_Verify = Bootloader_CRC_Verify((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_SIZE_BYTE, Host_CRC32);
	if(CRC_VERIFICATION_PASSED == CRC_Verify){
		Bootloader_Send_ACK(12);
		Bootloader_Send_Data_To_Host((uint8_t *)Bootloader_Support_CMDs, 12);
#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_print_message("CRC Verification Successful \r\n");
#endif
	}else{
		Bootloader_Send_NACK();
#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_print_message("CRC Verification failed \r\n");
#endif
	}

}
static void Bootloader_Get_Chip_Identification_Number(uint8_t *Host_Buffer){

}
static void Bootloader_Read_Protection_Level(uint8_t *Host_Buffer){

}
static void Bootloader_Jump_To_Address(uint8_t *Host_Buffer){

}
static void Bootloader_Erase_Flash(uint8_t *Host_Buffer){

}
static void Bootloader_Memory_Write(uint8_t *Host_Buffer){

}
static void Bootloader_Enable_RW_Protection(uint8_t *Host_Buffer){

}
static void Bootloader_Memory_Read(uint8_t *Host_Buffer){

}
static void Bootloader_Get_Sector_Protection_Status(uint8_t *Host_Buffer){

}
static void Bootloader_Read_OTP(uint8_t *Host_Buffer){

}
static void Bootloader_Disable_RW_Protection(uint8_t *Host_Buffer){

}

void BL_print_message(char *format, ... ){
	char Message[100]={0};
	va_list args;
	va_start(args, format);
	vsprintf(Message, format, args);
#if BL_DEBUG_METHOD == BL_ENABLE_UART_DEBUG_MESSAGE
	HAL_UART_Transmit(BL_DEBUG_UART,(uint8_t *)Message,sizeof(Message),HAL_MAX_DELAY);
#elif BL_DEBUG_METHOD == BL_ENABLE_SPI_DEBUG_MESSAGE
/* transmit from spi module*/

#elif BL_DEBUG_METHOD == BL_ENABLE_I2C_DEBUG_MESSAGE
/* transmit from i2c module*/

#else

#endif
	va_end(args);
}



