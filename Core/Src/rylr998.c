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
    char uartTxBuffer[20] = {0};  // Enough for "AT+NETWORKID=XX\r\n"

    // Validate the network ID
    if (((networkId >= 3) && (networkId <= 15)) || (networkId == 18)) {
        // Construct the AT command
        int packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+NETWORKID=%u\r\n", networkId);

        if (packetSize <= 0 || packetSize >= sizeof(uartTxBuffer)) {
            return HAL_ERROR;  // snprintf error or buffer overflow prevention
        }

        // Transmit the AT command over UART
        ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 10);
    }

    return ret;
}

/**
 * @brief  Sets the address for the RYLR998 module using the AT command. Saves in the FLASH
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  address: The address to be set on the RYLR998 module.
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_setAddress(UART_HandleTypeDef *puartHandle, uint16_t address){
	    HAL_StatusTypeDef ret = HAL_ERROR;
	    char uartTxBuffer[20] = {0};  // Enough size for "AT+ADDRESS=XXXXX\n"

	    // Construct the AT command
	    int packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+ADDRESS=%u\r\n", address);

	    if (packetSize <= 0 || packetSize >= sizeof(uartTxBuffer)) {
	        return HAL_ERROR;  // snprintf error or overflow prevention
	    }

	    // Transmit the AT command over UART
	    ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 10);

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
HAL_StatusTypeDef rylr998_setParameter(UART_HandleTypeDef *puartHandle, uint8_t SF, uint8_t BW, uint8_t CR, uint8_t ProgramedPreamble) {
    HAL_StatusTypeDef ret = HAL_ERROR;
    char uartTxBuffer[25] = {0};  // Enough size for "AT+PARAMETER=%u,%u,%u,%u\r\n"

    // Validate parameters
    if (SF < 5 || SF > 11) {
        return HAL_ERROR;  // Invalid Spreading Factor
    }
    if (BW < 7 || BW > 9) {
        return HAL_ERROR;  // Invalid Bandwidth
    }
    if (CR < 1 || CR > 4) {
        return HAL_ERROR;  // Invalid Coding Rate
    }
    if (ProgramedPreamble < 4 || ProgramedPreamble > 25) {
        return HAL_ERROR;  // Invalid Preamble Length
    }

    // Calculate the required buffer size
    int packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+PARAMETER=%u,%u,%u,%u\r\n", SF, BW, CR, ProgramedPreamble);

    // Ensure the packet size does not exceed the buffer size
    if (packetSize >= sizeof(uartTxBuffer)) {
        return HAL_ERROR;  // Buffer size exceeded
    }

    // Transmit the command over UART
    ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 10);

    return ret;
}


/**
 * @brief  Resets the RYLR998 module using the AT+RESET command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @retval HAL_StatusTypeDef: HAL_OK if the reset command is successfully transmitted, HAL_ERROR if memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_reset(UART_HandleTypeDef *puartHandle) {
	  HAL_StatusTypeDef ret = HAL_ERROR;
	    char uartTxBuffer[11] = {0};  // Enough size for "AT+RESET\r\n"

	    // Construct the AT command and check for formatting errors or overflow
	    int packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+RESET\r\n");

	    // Check if snprintf failed or if packetSize is larger than the buffer
	    if (packetSize < 0 || packetSize >= sizeof(uartTxBuffer)) {
	        return HAL_ERROR;  // snprintf error or buffer overflow
	    }

	    // Transmit the AT command over UART
	    ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 10);

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
    char uartTxBuffer[30] = {0};  //

    // Validate mode (0, 1, or 2)


    // Validate rxTime and LowSpeedTime (must be between 30 and 60000)
    if (mode==2){
		if (rxTime < 30 || rxTime > 60000) {
			return ret;
		}
		if (LowSpeedTime < 30 || LowSpeedTime > 60000) {
			return ret;
		}
    }else if(mode == 1 || mode==0){
    }else {
    	return ret;
    }
    // Calculate required buffer size dynamically (AT+MODE=<mode>,<rxTime>,<LowSpeedTime>\r\n)
    int packetSize;

    if (mode==1|| mode==0){
    	 packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+MODE=%u\r\n", mode);
    }else{
    	 packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+MODE=2,%lu,%lu\r\n", rxTime, LowSpeedTime);
    }
    // Allocate buffer
    if (packetSize <= 0 || packetSize >= sizeof(uartTxBuffer)) {
              return HAL_ERROR;  // snprintf error or buffer overflow prevention
          }

     ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 10);
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
    char uartTxBuffer[16] = {0};

    // Validate baud rate (optional - ensure it's a standard value)
    if (baudRate < 1200 || baudRate > 115200) {  // Adjust based on RYLR998 support
        return HAL_ERROR;  // Invalid baud rate
    }

    int packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+IPR=%lu\r\n", baudRate);

	if (packetSize <= 0 || packetSize >= sizeof(uartTxBuffer)) {
		return HAL_ERROR;  // snprintf error or overflow prevention
	}

	ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 10);

	return ret;
}


/**
 * @brief  Sets the frequency band for the RYLR998 module using the AT command.
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  frequency: The frequency to be set (valid range: 862000000-1020000000 Hz).
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if the frequency is invalid or memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_setBand(UART_HandleTypeDef *puartHandle, uint32_t frequency,uint8_t memory) { //TODO check if it works
    HAL_StatusTypeDef ret = HAL_ERROR;
    char uartTxBuffer[22] = {0};
    int packetSize=0;

    // Validate frequency (adjust based on RYLR998 supported bands)
    if (frequency < 862000000 || frequency > 1020000000) {
        return HAL_ERROR;  // Invalid frequency
    }

    // Construct the AT command and check for formatting errors or overflow
    if(memory){
	packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+BAND=%lu,M\r\n",frequency);
    }else{
    packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+BAND=%lu\r\n",frequency);
    }

	if (packetSize < 0 || packetSize >= sizeof(uartTxBuffer)) {
		return HAL_ERROR;  // snprintf error or buffer overflow
	}

	// Transmit the AT command over UART
	ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 10);

	return ret;
}



/**
 * @brief  Sets the PIN for the RYLR998 module using the AT command
 * @param  puartHandle: Pointer to the UART handle used for communication.
 * @param  password: Pointer to the 8-character password to be set,  from 00000000 to FFFFFFFF
 * @retval HAL_StatusTypeDef: HAL_OK if the command is successfully transmitted, HAL_ERROR if the password length is invalid or memory allocation fails.
 *
 */
HAL_StatusTypeDef rylr998_setCPIN(UART_HandleTypeDef *puartHandle, const char *password) {
    HAL_StatusTypeDef ret = HAL_ERROR;
    char uartTxBuffer[24] = {0};  // Aumenté el tamaño para mayor seguridad


    if (password == NULL || strlen(password) != 8) {
        return ret;
    }

    // Formatear el comando AT
    int packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+CPIN=%s\r\n", password);

    // Verificar si snprintf tuvo un error o si hubo un desbordamiento
    if (packetSize <= 0 || packetSize >= sizeof(uartTxBuffer)) {
        return HAL_ERROR;
    }

    // Enviar el comando AT por UART
    ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 100);

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
	    char uartTxBuffer[14] = {0};
	    if(CRFOP>22){
	    	return ret;
	    }


	    int packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+CRFOP=%u\r\n", CRFOP);




	  	if (packetSize <= 0 || packetSize >= sizeof(uartTxBuffer)) {
	  		return HAL_ERROR;  // snprintf error or overflow prevention
	  	}

	  	ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 10);

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

    char uartTxBuffer[12] = {0};


	int packetSize = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "AT+FACTORY\r\n");


	if (packetSize < 0 || packetSize >= sizeof(uartTxBuffer)) {
		return HAL_ERROR;
	}


	ret = HAL_UART_Transmit(puartHandle, (uint8_t*)uartTxBuffer, packetSize, 10);

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
	if(!memcmp(rxBuffer, "ADDRESS", 7))
	{
		return ret = RYLR_ADDRESS;
	}
	else if(!memcmp(rxBuffer, "+RCV", 4))
	{
		return ret = RYLR_RCV;
	}
	else if(!memcmp(rxBuffer, "+OK\r\n", 5))
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


RYLR_RX_data_t rx_packet;

RYLR_RX_command_t rylr998_prase_reciver(uint8_t *pBuff, uint8_t RX_BUFFER_SIZE)
{

	static uint8_t aux_buff[255];
	static uint8_t start_indx=0;
	static uint8_t i;

	if(pBuff[start_indx]=='\n'){   //RYLR_IPR returns +RYLR_IPR=115200\r\n\n for some reason so this fixes it
									//TODO: improvement: search for the '+' in ASCII and then, save it in the aux_buff while keeping track of the index
		start_indx=start_indx+1;
	}

	for (i = 0; i <RX_BUFFER_SIZE; i++){
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
                	 *  +RCV=50,5,HELLO,-99,40\r\n
                	 */

            	    char *token;


            	    // Parse ID address
            	    token = strtok(aux_buff, "=");  // Remove "+RCV="
            	    token = strtok(NULL, ",");      // Get ID address
            	    rx_packet.id = atoi(token);

            	    // Parse byte count
            	    token = strtok(NULL, ",");      // Get byte count
            	    rx_packet.byte_count = atoi(token);

            	    // Parse actual data
            	    token = strtok(NULL, ",");
            	    strncpy(rx_packet.data, token, rx_packet.byte_count);  // Copy up to byte_count
            	    rx_packet.data[rx_packet.byte_count] = '\0';  // Ensure null termination

            	    // Parse RSSI
            	    token = strtok(NULL, ",");      // Get RSSI
            	    rx_packet.rssi = atoi(token);

            	    // Parse SNR
            	    token = strtok(NULL, ",");      // Get SNR
            	    rx_packet.snr = atoi(token);

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
            return cmd;
}

