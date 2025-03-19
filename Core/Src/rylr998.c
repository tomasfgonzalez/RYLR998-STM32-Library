/*
 * rylr998.c
 *
 *  Created on: Mar 14, 2025
 *      Author: Tomas Francisco Gonzalez
 */
#include "rylr998.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




/**
 * @brief  Sends data to a specific address on the RYLR998 module using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  address: The destination address for the data.
 * @param  data: Pointer to the data to be sent.
 * @param  data_length: Length of the data to be sent.
 * @retval HAL_StatusTypeDef: HAL_OK if UART transmission is successful, HAL_ERROR if failed.
 */
HAL_StatusTypeDef rylr998_sendData(UART_HandleTypeDef *puartHandle, uint16_t address, uint8_t *data, uint8_t data_length) {
    HAL_StatusTypeDef ret = HAL_ERROR;
    uint16_t packetSize = snprintf(NULL, 0, "AT+SEND=%u,%u,", address, data_length) + data_length + 2;  // +2 for \r\n

    // Allocate buffer dynamically
    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
    if (uartTxBuffer == NULL) {
        return HAL_ERROR;  // Allocation failed
    }

    // Construct the AT command
    uint16_t offset = snprintf((char*)uartTxBuffer, packetSize, "AT+SEND=%u,%u,", address, data_length);

    // Append data
    memcpy(uartTxBuffer + offset, data, data_length);
    offset += data_length;

    // Append command terminator
    uartTxBuffer[offset++] = '\r';
    uartTxBuffer[offset++] = '\n';

    // Transmit command over UART
    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, offset);

    // Free allocated memory
    free(uartTxBuffer);

    return ret;
}



/**
 * @brief  Sets the network ID for the RYLR998 module using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  networkId: The network ID to be set (valid range: 3-15, 18).
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if validation fails or memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_networkId(UART_HandleTypeDef *puartHandle, uint8_t networkId) {
    HAL_StatusTypeDef ret = HAL_ERROR;

    // Validate network ID (Allowed: 3-15, 18)
     if (networkId < 3 || networkId == 16 || networkId == 17 || networkId > 18) {
         return ret;  // Invalid network ID, return error
     }

    // Calculate required buffer size dynamically
    uint16_t packetSize = snprintf(NULL, 0, "AT+NETWORKID=%u\r\n", networkId) + 1;

    // Allocate buffer
    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
    if (uartTxBuffer == NULL) {
        return HAL_ERROR;  // Memory allocation failed
    }

    // Construct the AT command
    snprintf((char*)uartTxBuffer, packetSize, "AT+NETWORKID=%u\r\n", networkId);

    // Transmit the command over UART
    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize - 1);  // -1 to exclude null terminator

    // Free allocated memory
    free(uartTxBuffer);

    return ret;
}


/**
 * @brief  Sets the address for the RYLR998 module using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  address: The address to be set on the RYLR998 module.
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_setAddress(UART_HandleTypeDef *puartHandle, uint16_t address){
	    HAL_StatusTypeDef ret = HAL_ERROR;

	    // Calculate required buffer size dynamically
	    uint16_t packetSize = snprintf(NULL, 0, "AT+ADDRESS=%u\r\n", address) + 1;

	    // Allocate buffer
	    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
	    if (uartTxBuffer == NULL) {
	        return HAL_ERROR;  // Memory allocation failed
	    }

	    // Construct the AT command
	    snprintf((char*)uartTxBuffer, packetSize, "AT+ADDRESS=%u\r\n", address);

	    // Transmit the command over UART
	    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize - 1);  // -1 to exclude null terminator

	    // Free allocated memory
	    free(uartTxBuffer);

	    return ret;
}

/**
 * @brief  Sets the RYLR998 module's parameters using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  SF: Spreading Factor (valid range: 5-11).
 * @param  BW: Bandwidth (valid range: 7-9).
 * @param  CR: Coding Rate (valid range: 1-4).
 * @param  ProgramedPreamble: Programmed preamble length (valid range: 4-25).
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if validation fails or memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_setParameter(UART_HandleTypeDef *puartHandle,uint8_t SF,uint8_t BW,uint8_t CR,uint8_t ProgramedPreamble){
	 HAL_StatusTypeDef ret = HAL_ERROR;
	 	 	if(SF<5 ||SF>11){
	 	 		return ret;
	 	 	}
	 	 	if(BW<7||BW>9){
	 	 		return ret;
	 	 	}
	 	 	if(CR<1||CR>4){
	 	 		return ret;
	 	 	}
	 	 	if(ProgramedPreamble<4 || ProgramedPreamble>25){
	 	 		return ret;
	 	 	}
		    // Calculate required buffer size dynamically
		    uint16_t packetSize = snprintf(NULL, 0, "AT+PARAMETER=%u,%u,%u,%u\r\n", SF,BW,CR,ProgramedPreamble) + 1;

		    // Allocate buffer
		    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
		    if (uartTxBuffer == NULL) {
		        return HAL_ERROR;  // Memory allocation failed
		    }

		    // Construct the AT command
		    snprintf((char*)uartTxBuffer, packetSize, "AT+PARAMETER=%u,%u,%u,%u\r\n", SF,BW,CR,ProgramedPreamble);

		    // Transmit the command over UART
		    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize - 1);  // -1 to exclude null terminator

		    // Free allocated memory
		    free(uartTxBuffer);

		    return ret;

}

/**
 * @brief  Resets the RYLR998 module using the AT+RESET command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @retval HAL_StatusTypeDef: HAL_OK if the reset command is successfully transmitted, HAL_ERROR if memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_reset(UART_HandleTypeDef *puartHandle) { //TODO check if it works
    HAL_StatusTypeDef ret = HAL_ERROR;

    // Define the AT command
    const char *resetCmd = "AT+RESET\r\n";
    uint16_t packetSize = strlen(resetCmd);

    // Allocate buffer
    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
    if (uartTxBuffer == NULL) {
        return HAL_ERROR;  // Memory allocation failed
    }

    // Copy command into the buffer
    memcpy(uartTxBuffer, resetCmd, packetSize);

    // Transmit the command over UART
    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize);

    // Free allocated memory
    free(uartTxBuffer);

    return ret;
}


/**
 * @brief  Sets the RYLR998 module's operating mode using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  mode: The mode to be set (valid values: 0, 1, or 2).
 * @param  rxTime: The receive time in milliseconds (valid range: 30-60000).
 * @param  LowSpeedTime: The low-speed time in milliseconds (valid range: 30-60000).
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if validation fails or memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_mode(UART_HandleTypeDef *puartHandle, uint8_t mode, uint32_t rxTime, uint32_t LowSpeedTime) {//TODO check if it works
    HAL_StatusTypeDef ret = HAL_ERROR;

    // Validate mode (0, 1, or 2)
    if (mode > 2) {
        return ret;
    }

    // Validate rxTime and LowSpeedTime (must be between 30 and 60000)
    if (rxTime < 30 || rxTime > 60000) {
        return ret;
    }
    if (LowSpeedTime < 30 || LowSpeedTime > 60000) {
        return ret;
    }

    // Calculate required buffer size dynamically (AT+MODE=<mode>,<rxTime>,<LowSpeedTime>\r\n)
    uint16_t packetSize = snprintf(NULL, 0, "AT+MODE=%u,%lu,%lu\r\n", mode, rxTime, LowSpeedTime) + 1;

    // Allocate buffer
    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
    if (uartTxBuffer == NULL) {
        return HAL_ERROR;  // Memory allocation failed
    }

    // Construct the AT command with proper formatting
    snprintf((char*)uartTxBuffer, packetSize, "AT+MODE=%u,%lu,%lu\r\n", mode, rxTime, LowSpeedTime);

    // Transmit the command over UART
    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize - 1);  // -1 to exclude null terminator

    // Free allocated memory
    free(uartTxBuffer);

    return ret;
}

/**
 * @brief  Sets the baud rate for the RYLR998 module using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  baudRate: The baud rate to be set (valid range: 1200-115200).
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if the baud rate is invalid or memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_setBaudRate(UART_HandleTypeDef *puartHandle, uint32_t baudRate) { //TODO check if it works
    HAL_StatusTypeDef ret = HAL_ERROR;

    // Validate baud rate (optional - ensure it's a standard value)
    if (baudRate < 1200 || baudRate > 115200) {  // Adjust based on RYLR998 support
        return HAL_ERROR;  // Invalid baud rate
    }

    // Calculate required buffer size dynamically
    uint16_t packetSize = snprintf(NULL, 0, "AT+IPR=%lu\r\n", baudRate) + 1;

    // Allocate buffer
    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
    if (uartTxBuffer == NULL) {
        return HAL_ERROR;  // Memory allocation failed
    }

    // Construct the AT command
    snprintf((char*)uartTxBuffer, packetSize, "AT+IPR=%lu\r\n", baudRate);

    // Transmit the command over UART
    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize - 1);  // -1 to exclude null terminator

    // Free allocated memory
    free(uartTxBuffer);

    return ret;
}

/**
 * @brief  Sets the frequency band for the RYLR998 module using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  frequency: The frequency to be set (valid range: 862000000-1020000000 Hz).
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if the frequency is invalid or memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_setBand(UART_HandleTypeDef *puartHandle, uint32_t frequency) { //TODO check if it works
    HAL_StatusTypeDef ret = HAL_ERROR;

    // Validate frequency (adjust based on RYLR998 supported bands)
    if (frequency < 862000000 || frequency > 1020000000) {
        return HAL_ERROR;  // Invalid frequency
    }

    // Calculate required buffer size dynamically
    uint16_t packetSize = snprintf(NULL, 0, "AT+BAND=%lu\r\n", frequency) + 1;

    // Allocate buffer
    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
    if (uartTxBuffer == NULL) {
        return HAL_ERROR;  // Memory allocation failed
    }

    // Construct the AT command
    snprintf((char*)uartTxBuffer, packetSize, "AT+BAND=%lu\r\n", frequency);

    // Transmit the command over UART
    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize - 1);  // -1 to exclude null terminator

    // Free allocated memory
    free(uartTxBuffer);

    return ret;
}



/**
 * @brief  Sets the PIN for the RYLR998 module using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  password: Pointer to the 8-character password to be set.
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if the password length is invalid or memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_setCPIN(UART_HandleTypeDef *puartHandle, uint8_t *password) { //TODO check if it works
    HAL_StatusTypeDef ret = HAL_ERROR;

    // Validate password length (should be 8 characters)
    if (strlen((char*)password) != 8) {
        return ret;  // Invalid password length
    }

    // Calculate the required buffer size for the AT command (AT+CPIN=<password>\r\n)
    uint16_t packetSize = snprintf(NULL, 0, "AT+CPIN=%s\r\n", password) + 1;

    // Allocate buffer dynamically
    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
    if (uartTxBuffer == NULL) {
        return HAL_ERROR;  // Memory allocation failed
    }

    // Construct the AT command
    snprintf((char*)uartTxBuffer, packetSize, "AT+CPIN=%s\r\n", password);

    // Transmit command over UART using DMA
    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize - 1);  // Exclude null terminator

    // Free allocated memory
    free(uartTxBuffer);

    return ret;
}

/**
 * @brief  Sets the CRFOP (Coding Rate and Frequency Offset) value for the RYLR998 module using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  CRFOP: The CRFOP value to be set (must be between 0 and 22).
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if the CRFOP value is invalid or memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_setCRFOP(UART_HandleTypeDef *puartHandle, uint8_t CRFOP){ //TODO check if it works
	    HAL_StatusTypeDef ret = HAL_ERROR;

	    if(CRFOP>22){
	    	return ret;
	    }

	    // Calculate required buffer size dynamically
	    uint16_t packetSize = snprintf(NULL, 0, "AT+CRFOP=%u\r\n", CRFOP) + 1;


	    // Allocate buffer
	    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
	    if (uartTxBuffer == NULL) {
	        return HAL_ERROR;  // Memory allocation failed
	    }

	    // Construct the AT command
	    snprintf((char*)uartTxBuffer, packetSize, "AT+CRFOP=%u\r\n", CRFOP);

	    // Transmit the command over UART
	    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize - 1);  // -1 to exclude null terminator

	    // Free allocated memory
	    free(uartTxBuffer);

	    return ret;
}

/**
 * @brief  Resets the RYLR998 module to its factory default settings using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_FACTORY(UART_HandleTypeDef *puartHandle) {
    HAL_StatusTypeDef ret = HAL_ERROR;

    // Calculate required buffer size dynamically
    uint16_t packetSize = snprintf(NULL, 0, "AT+FACTORY\r\n") + 1;

    // Allocate buffer
    uint8_t *uartTxBuffer = (uint8_t *)malloc(packetSize);
    if (uartTxBuffer == NULL) {
        return HAL_ERROR;  // Memory allocation failed
    }

    // Construct the AT command
    snprintf((char*)uartTxBuffer, packetSize, "AT+FACTORY\r\n");

    // Transmit the command over UART
    ret = HAL_UART_Transmit_DMA(puartHandle, uartTxBuffer, packetSize - 1);  // -1 to exclude null terminator

    // Free allocated memory
    free(uartTxBuffer);

    return ret;
}


uint8_t rylr998_interrupt_flag;


/**
 * @brief  Sets IRQ flag whenever new data gets into gets recived in the Rx buffer
 *
 */
void rylr998_SetInterruptFlag(void){
	rylr998_interrupt_flag =1;
}



/**
 * @brief  Returns the value of the flag
 * @retval flag status
 *
 */
uint8_t rylr998_GetInterruptFlag(void){
	return rylr998_interrupt_flag;
}


/**
 * @brief  Clear the IRQ flag
 *
 */
void rylr998_ClearInterruptFlag(void){
	rylr998_interrupt_flag =0;
}








/**
 * @brief handles the response of the uart msg
 * @params RxBuffer sorted
 * @retval command selected
 *
 */
RYLR_RX_command_t rylr998_ResponseFind(uint8_t *rxBuffer)
{

	RYLR_RX_command_t 	ret 					= RYLR_NOT_FOUND;
	if(!memcmp(rxBuffer, "ADDRESS\r\n", 9))
	{
		return ret = RYLR_ADDRESS;
	}
	else if(!memcmp(rxBuffer, "+RCV", 4))
	{
		return ret = RYLR_RCV;
	}
	else if(!memcmp(rxBuffer, "OK+\r\n", 5))
	{
		return ret = RYLR_OK;
	}
	else if(!memcmp(rxBuffer, "+READY\r\n", 8))
	{
		return ret = RYLR_RDY;
	}
	else if(!memcmp(rxBuffer, "+ERR=", 5))
	{
		return ret = RYLR_ERR;
	}
	else if(!memcmp(rxBuffer, "+FACTORY\r\n", 10))
	{
		return ret = RYLR_FACTORY;
	}
	else if(!memcmp(rxBuffer, "+IPR=", 5))
	{
		return ret = RYLR_IPR;
	}
	return ret;
}



void rylr998_prase_reciver(uint8_t *pBuff, uint8_t RX_BUFFER_SIZE)
{
	static uint8_t aux_buff[32];
	static uint8_t start_indx=0;
	uint8_t i;
	for (i == 0; i <RX_BUFFER_SIZE; i++){
	    aux_buff[i] = pBuff[(start_indx + i) % RX_BUFFER_SIZE];
	    if(aux_buff[i]=='\n'){
	    	rylr998_ClearInterruptFlag();
	    	break;
	    }
	    if(i==RX_BUFFER_SIZE && aux_buff[i]!='\n'){
	    }


	}
	start_indx=(start_indx + i+1) % RX_BUFFER_SIZE;
	i=0;



            RYLR_RX_command_t cmd = rylr998_ResponseFind(aux_buff);

            // Handle different cases
            switch (cmd)
            {
                case RYLR_ADDRESS:
                    // Handle ADDRESS response
                    break;
                case RYLR_RCV:
                    // Handle RCV response
                	/*Example: Module received the ID Address 50 send 5 bytes data,
                	 * Content is HELLO string, RSSI is -99dBm, SNR is 40, It will show as below.
                	 *  +RCV=50,5,HELLO,-99,40“\r\n
                	 */

                	/*
                	for (int i=4;i<data_size;i++){
                		//first char should be "="
                		uint8_t recived_address_counter;
                		if(aux_buff[i] == '='){
                			recived_address_counter++;
                		}

*/






                    break;
                case RYLR_OK:
                    // Handle OK response
                    break;
                case RYLR_RDY:
                    // Handle READY response
                    break;
                case RYLR_ERR:

                	while(1){  //TODO  for now, if something went wrong, the code gets stuck here.
                		Error_Handler();
                	}
                	break;
                default:
                    break;
            }

            rylr998_ClearInterruptFlag();
}

