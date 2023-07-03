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

/*---------------------- Section : Global Variables Definitions ------- */
static uint8_t BL_Host_Buffer[BL_HOST_BUFFER_RC_LENGTH];


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
					BL_print_message("Read the bootloader version from the MCU \r\n");
					Bootloader_Get_Version(BL_Host_Buffer);
					Status = BL_OK;
					break;
				case CBL_GET_HELP_CMD:
					BL_print_message("Read the commands supported by the bootloader \r\n");
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

static void Bootloader_Get_Version(uint8_t *Host_Buffer){

}
static void Bootloader_Get_Help(uint8_t *Host_Buffer){

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



