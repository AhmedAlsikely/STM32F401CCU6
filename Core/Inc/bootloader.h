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
#define BL_HOST_COMMUNICATION_UART    &huart1
#define BL_CRC_ENABLE_OBJ             &hcrc

#define BL_ENABLE_UART_DEBUG_MESSAGE 0x00
#define BL_ENABLE_SPI_DEBUG_MESSAGE  0x01
#define BL_ENABLE_I2C_DEBUG_MESSAGE  0x02

#define BL_DEBUG_METHOD BL_ENABLE_UART_DEBUG_MESSAGE

#define BL_HOST_BUFFER_RC_LENGTH 200

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
#define CBL_SW_MINOR_VERSION          0
#define CBL_SW_PATCH_VERSION          0

#define CRC_SIZE_BYTE                 4

#define CRC_VERIFICATION_FAILED       0x00
#define CRC_VERIFICATION_PASSED       0x01

#define CBL_SEND_ACK                  0xCD
#define CBL_SEND_NACK                 0xAB

/*---------------------- Section : Macro Functions Declarations - */

/*---------------------- Section : Data Type Declarations ------- */
typedef enum{
	BL_NACK =0,
	BL_OK
}BL_Status;

/*---------------------- Section : Functions Declarations ------- */
void BL_print_message(char *format, ... );
BL_Status BL_UART_Featch_Host_Command(void);

#endif /* INC_BOOTLOADER_H_ */
