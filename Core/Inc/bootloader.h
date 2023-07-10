/*
 * bootloader.h
 *
 *  Created on: Jul 3, 2023
 *      Author: Alsikely
 */

#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_
/*---------------------- Section : Includes --------------------- */
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "usart.h"
#include "crc.h"


/*---------------------- Section : Macro Declarations------------ */
#define BL_DEBUG_UART                 &huart1
#define BL_HOST_COMMUNICATION_UART    &huart2
#define BL_CRC_ENABLE_OBJ             &hcrc

#define DEBUG_INFO_DISABLE            0x00
#define DEBUG_INFO_ENABLE             0x01
#define BL_DEBUG_ENABLE               DEBUG_INFO_ENABLE

#define BL_ENABLE_UART_DEBUG_MESSAGE  0x00
#define BL_ENABLE_SPI_DEBUG_MESSAGE   0x01
#define BL_ENABLE_I2C_DEBUG_MESSAGE   0x02

#define BL_DEBUG_METHOD BL_ENABLE_UART_DEBUG_MESSAGE

#define BL_HOST_BUFFER_RC_LENGTH 256

#define CBL_GET_VER_CMD               0x10
#define CBL_GET_HELP_CMD              0x11
#define CBL_GET_CID_CMD               0x12
#define CBL_GET_RDP_STATUS_CMD        0x13
#define CBL_GO_TO_ADDR_CMD            0x14
#define CBL_FLASH_ERASE_CMD           0x15
#define CBL_MEM_WRITE_CMD             0x16
#define CBL_EN_R_W_PROTECT_CMD        0x17
#define CBL_MEM_READ_CMD              0x18
#define CBL_READ_SECTOR_STATUS_CMD    0x19
#define CBL_OTP_READ_CMD              0x20
#define CBL_DIS_R_W_PROTECT_CMD       0x21

#define CBL_VENDOR_ID                 100
#define CBL_SW_MAJOR_VERSION          1
#define CBL_SW_MINOR_VERSION          5
#define CBL_SW_PATCH_VERSION          3

#define CRC_SIZE_BYTE                 4

#define CRC_VERIFICATION_FAILED       0x00
#define CRC_VERIFICATION_PASSED       0x01

#define CBL_SEND_ACK                  0xCD
#define CBL_SEND_NACK                 0xAB

/* Start address of sector 2 */
#define FLASH_SECTOR2_BASE_ADDRESS    0x0800C000

#define ADDRESS_IS_INVALID            0x00
#define ADDRESS_IS_VALID              0x01

#define STM32F401_FLASH_SIZE 		  (256 * 1024)
#define STM32F401_SRAM1_SIZE 		  (64  * 1024)
#define STM32F401_FLASH_END		      (FLASH_BASE + STM32F401_FLASH_SIZE)
#define STM32F401_SRAM1_END		      (SRAM1_BASE + STM32F401_SRAM1_SIZE)

/* CBL_FLASH_ERASE_CMD */
#define CBL_FLASH_MAX_SECTOR_NUMBER   0x08
#define CBL_FLASH_MASS_ERASE          0xFF

#define INVAlID_SECTOR_NUMBER         0x00
#define VAlID_SECTOR_NUMBER           0x01
#define SECTOR_ERASE_FAILED           0x02
#define SECTOR_ERASE_SUCCESS          0x03

#define FALSH_SUCCESSFUL_ERASE 		  0xFFFFFFFFU

/* CBL_MEM_WRITE_CMD */
#define FLASH_PAYLOAD_WRITE_FAILED 	  0x00
#define FLASH_PAYLOAD_WRITE_PASSED    0x01

/* CBL_DIS_R_W_PROTECT_CMD */
#define ROP_LEVEL_CHANGE_INVALID        0x00
#define ROP_LEVEL_CHANGE_VALID          0x01

#define CBL_ROP_LEVEL_0							0x00
#define CBL_ROP_LEVEL_1							0x01
#define CBL_ROP_LEVEL_2							0x02

/*---------------------- Section : Macro Functions Declarations - */

/*---------------------- Section : Data Type Declarations ------- */
typedef enum{
	BL_NACK =0,
	BL_OK
}BL_Status;

typedef void (*pMainApp)(void);
typedef void (*Jump_ptr)(void);
/*---------------------- Section : Functions Declarations ------- */
void BL_print_message(char *format, ... );
BL_Status BL_UART_Featch_Host_Command(void);

#endif /* INC_BOOTLOADER_H_ */
