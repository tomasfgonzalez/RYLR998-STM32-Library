/*
 * rylr998.h
 *
 *  Created on: Mar 14, 2025
 *      Author: Tomas Francisco Gonzalez
 */

#ifndef INC_RYLR998_H_
#define INC_RYLR998_H_

#include "stm32l0xx_hal.h"



/*
 * +RCV=<Address>,<Length>,<Data>,<RSSI>,<SNR>,
	<Address> Transmitter Address ID
	<Length> Data Length
	<Data> ASCll Data
	<RSSI> Received Signal Strength
	<SNR> Signal-to-noise ratio
 */


typedef enum
{
	RYLR_OK = 0x00U,
	RYLR_ADDRESS,
	RYLR_RCV,
	RYLR_RDY,
	RYLR_IPR,
	RYLR_UID,
	RYLR_VER,
	RYLR_FACTORY,
	RYLR_RESET,
	RYLR_READY,
	RYLR_ERR,
	RYLR_NOT_FOUND

} RYLR_RX_command_t;

typedef struct{
	uint16_t id;
	uint8_t byte_count;
	uint8_t data[240];
	int8_t rssi;
	uint8_t snr;
}RYLR_RX_data_t;


typedef struct
{
	uint8_t 			address[2];
	uint8_t				payloadLength;
	uint8_t				rxBuffer[240];
	int8_t				rssi;
	uint8_t				snr;

}RYLR_reciver_t;

extern RYLR_RX_data_t rx_packet;
extern RYLR_reciver_t rxDataBuff;

//Tx
HAL_StatusTypeDef rylr998_sendData(UART_HandleTypeDef *uartHandle,uint16_t address, uint8_t *data,uint8_t data_length);//uses MALLOC
HAL_StatusTypeDef rylr998_networkId(UART_HandleTypeDef *puartHandle, uint8_t networkId);
HAL_StatusTypeDef rylr998_setAddress(UART_HandleTypeDef *puartHandle, uint16_t address);
HAL_StatusTypeDef rylr998_setParameter(UART_HandleTypeDef *puartHandle,uint8_t SF,uint8_t BW,uint8_t CR,uint8_t ProgramedPreamble);
HAL_StatusTypeDef rylr998_reset(UART_HandleTypeDef *puartHandle);
HAL_StatusTypeDef rylr998_mode(UART_HandleTypeDef *puartHandle,uint8_t mode,uint32_t rxTime,uint32_t LowSpeedTime);
HAL_StatusTypeDef rylr998_setBaudRate(UART_HandleTypeDef *puartHandle, uint32_t baudRate);
HAL_StatusTypeDef rylr998_setBand(UART_HandleTypeDef *puartHandle, uint32_t frequency,uint8_t memory);
HAL_StatusTypeDef rylr998_setCPIN(UART_HandleTypeDef *puartHandle, const char *password);
HAL_StatusTypeDef rylr998_setCRFOP(UART_HandleTypeDef *puartHandle, uint8_t CRFOP);
HAL_StatusTypeDef rylr998_FACTORY(UART_HandleTypeDef *puartHandle);
//TODO AT+UID?
//TODO AT+VER?


//Rx
RYLR_RX_command_t rylr998_prase_reciver(uint8_t *pBuff,uint8_t RX_BUFFER_SIZE);
RYLR_RX_command_t rylr998_ResponseFind(uint8_t *rxBuffer);


void rylr998_SetInterruptFlag(void);
uint8_t rylr998_GetInterruptFlag(void);
void rylr998_ClearInterruptFlag(void);


#endif /* INC_RYLR998_H_ */
