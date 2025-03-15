/*
 * rylr998.h
 *
 *  Created on: Mar 14, 2025
 *      Author: Tomas Francisco Gonzalez
 */

#ifndef INC_RYLR998_H_
#define INC_RYLR998_H_

#include "stm32l0xx_hal.h"


HAL_StatusTypeDef rylr998_sendData(UART_HandleTypeDef *uartHandle,uint16_t address, uint8_t *data,uint8_t data_length);
HAL_StatusTypeDef rylr998_networkId(UART_HandleTypeDef *puartHandle, uint8_t networkId);
HAL_StatusTypeDef rylr998_setAddress(UART_HandleTypeDef *puartHandle, uint16_t address);
HAL_StatusTypeDef rylr998_setParameter(UART_HandleTypeDef *puartHandle,uint8_t SF,uint8_t BW,uint8_t CR,uint8_t ProgramedPreamble);
HAL_StatusTypeDef rylr998_reset(UART_HandleTypeDef *puartHandle);
HAL_StatusTypeDef rylr998_mode(UART_HandleTypeDef *puartHandle,uint8_t mode,uint32_t rxTime,uint32_t LowSpeedTime);
HAL_StatusTypeDef rylr998_setBaudRate(UART_HandleTypeDef *puartHandle, uint32_t baudRate);
HAL_StatusTypeDef rylr998_setBand(UART_HandleTypeDef *puartHandle, uint32_t frequency);
HAL_StatusTypeDef rylr998_setCPIN(UART_HandleTypeDef *puartHandle, uint8_t *password);
HAL_StatusTypeDef rylr998_setCRFOP(UART_HandleTypeDef *puartHandle, uint8_t CRFOP);

#endif /* INC_RYLR998_H_ */
