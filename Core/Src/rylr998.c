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

void rylr998_setChannel(uint8_t ch,uint8_t address){
	RYLR_config_t config_handler;

	if(ch){
	config_handler.networkId =18;
	config_handler.address =address;
	config_handler.SF=9;
	config_handler.BW=7;
	config_handler.CR=1;
	config_handler.ProgramedPreamble=12;
	config_handler.mode=0;
	config_handler.rxTime=0;
	config_handler.LowSpeedTime=0;
	//config_handler.baudRate=115200;
	config_handler.frequency=915000000;
	config_handler.memory=1;
	strcpy(config_handler.password, "FFFFFFFF"); //we dont want the \0 terminator so we overflow
	config_handler.CRFOP=22;
	}else{
	config_handler.networkId =18;
	config_handler.address =address;
	config_handler.SF=9;
	config_handler.BW=7;
	config_handler.CR=1;
	config_handler.ProgramedPreamble=12;
	config_handler.mode=0;
	config_handler.rxTime=0;
	config_handler.LowSpeedTime=0;
	//config_handler.baudRate=115200;
	config_handler.frequency=915000000;
	config_handler.memory=1;
	strcpy(config_handler.password, "FFFFFFFF"); //we dont want the \0 terminator so we overflow
	config_handler.CRFOP=22;
	}
	rylr998_config(&config_handler);
}



void rylr998_config(RYLR_config_t *config_handler){
		rylr998_FACTORY();
		rylr998_getCommand(RYLR_FACTORY,rx_buff,RX_BUFF);
		//NETWORKID
		rylr998_networkId(config_handler->networkId);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//ADDRESS
		rylr998_setAddress(config_handler->address);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//PARAMETERS
		rylr998_setParameter(config_handler->SF, config_handler->BW, config_handler->CR, config_handler->ProgramedPreamble);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//MODE
		rylr998_mode(config_handler->mode,config_handler->rxTime,config_handler->LowSpeedTime);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//BaudRate
		//rylr998_setBaudRate(config_handler->baudRate);
		//rylr998_getCommand(RYLR_IPR,rx_buff,RX_BUFFER_SIZE); //ADD RYLR_IPR
		//FREQ Band
		rylr998_setBand(config_handler->frequency,config_handler->memory);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//PASSWORD
		rylr998_setCPIN(config_handler->password);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
		//RF Output
		rylr998_setCRFOP(config_handler->CRFOP);
		rylr998_getCommand(RYLR_OK,rx_buff,RX_BUFF);
}




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



#define TX_BUFFER_SIZE 64
static char uartTxBuffer[TX_BUFFER_SIZE];

void rylr998_sendCommand(const char *cmd) {
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)cmd, strlen(cmd), 20);
}

void rylr998_getCommand(RYLR_RX_command_t cmd,uint8_t *rx_buff,uint8_t RX_BUFFER_SIZE){
	HAL_Delay(30);
		if(rylr998_GetInterruptFlag()){
				if(rylr998_prase_reciver(rx_buff,RX_BUFFER_SIZE)!=cmd){
				}
		}else{
			Error_Handler();
		}
}

void rylr998_setAddress(uint8_t address) {
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
    snprintf(uartTxBuffer, sizeof(uartTxBuffer), AT "ADDRESS=%d" END, address);
    rylr998_sendCommand(uartTxBuffer);
}

void rylr998_networkId(uint8_t networkId){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT "NETWORKID=%u" END, networkId);
	rylr998_sendCommand(uartTxBuffer);
}

void rylr998_setParameter(uint8_t SF, uint8_t BW, uint8_t CR, uint8_t ProgramedPreamble){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT "PARAMETER=%u,%u,%u,%u" END, SF, BW, CR, ProgramedPreamble);
	rylr998_sendCommand(uartTxBuffer);
}
/*
void rylr998_reset(void){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT "RESET" END);
	rylr998_sendCommand(uartTxBuffer);
}*/

void rylr998_mode(uint8_t mode, uint32_t rxTime, uint32_t LowSpeedTime){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	if (rxTime==0||LowSpeedTime==0){snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT"MODE=%u" END, mode);
	}else{snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT"MODE=2,%lu,%lu" END,rxTime,LowSpeedTime);}
	rylr998_sendCommand(uartTxBuffer);
}

/*
void rylr998_setBaudRate(uint32_t baudRate){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE,  AT "IPR=%lu" END, baudRate);
	rylr998_sendCommand(uartTxBuffer);
}*/

void rylr998_setBand(uint32_t frequency,uint8_t memory){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	if(memory){snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT"BAND=%lu,M"END,frequency);
	}else{snprintf(uartTxBuffer, TX_BUFFER_SIZE, AT"BAND=%lu,M"END,frequency);}
	rylr998_sendCommand(uartTxBuffer);
}

void rylr998_setCPIN(const char *password){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE,  AT "CPIN=%s"END, password);
	rylr998_sendCommand(uartTxBuffer);
}

void rylr998_setCRFOP(uint8_t CRFOP){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE,  AT"CRFOP=%u"END, CRFOP);
	rylr998_sendCommand(uartTxBuffer);
}

void rylr998_FACTORY(void){
	memset(uartTxBuffer, 0, sizeof(TX_BUFFER_SIZE));
	snprintf(uartTxBuffer, TX_BUFFER_SIZE,  AT"FACTORY"END);
	rylr998_sendCommand(uartTxBuffer);
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
RYLR_RX_command_t rylr998_ResponseFind(char *rxBuffer)
{

	RYLR_RX_command_t 	ret 					= RYLR_NOT_FOUND;
	if(!memcmp(rxBuffer, "+OK"END, 5))
	{
		return ret = RYLR_OK;
	}
	else if(!memcmp(rxBuffer, "+RCV", 4))
		{
			return ret = RYLR_RCV;
		}
	else if(!memcmp(rxBuffer, "+ERR=", 5))
		{
			return ret = RYLR_ERR;
		}
	/*else if(!memcmp(rxBuffer, "+READY"END, 5))
	{
		return ret = RYLR_RDY;
	}
	else if(!memcmp(rxBuffer, "+FACTORY", 8))
	{
		return ret = RYLR_FACTORY;
	}
	else if(!memcmp(rxBuffer, "+IPR=", 5))
	{
		return ret = RYLR_IPR;
	}*/
	return ret;
}


RYLR_RX_data_t rx_packet;

RYLR_RX_command_t rylr998_prase_reciver(uint8_t *pBuff, uint8_t RX_BUFFER_SIZE)
{

	static char aux_buff[64];  //it must match with RX_BUFFER_SIZE
	static uint8_t start_indx=0;
	static uint8_t i;

	for(i = 0; i <RX_BUFFER_SIZE; i++){   //Looks for the index of the starting char

		if(pBuff[(start_indx+i) % RX_BUFFER_SIZE]=='+'){
			start_indx=(start_indx + i) % RX_BUFFER_SIZE;
			break;
		}
	}

	for (i = 0; i <RX_BUFFER_SIZE; i++){

		aux_buff[i] = pBuff[(start_indx + i) % RX_BUFFER_SIZE];

		if(aux_buff[i]=='\n'){
			break;
		}
	}
	rylr998_ClearInterruptFlag();
	start_indx=(start_indx + i+1) % RX_BUFFER_SIZE;

            RYLR_RX_command_t cmd = rylr998_ResponseFind(aux_buff);

            // Handle different cases
            switch (cmd)
            {
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
                case RYLR_ERR:

                	while(1){  //TODO  for now, if something went wrong, the code gets stuck here.
                		Error_Handler();
                	}
                	break;
                /*
                case RYLR_RDY:
                    // Handle READY response
                    break;
                case RYLR_ADDRESS:
                                                   // Handle ADDRESS response
                    break;*/
                default:
                    break;
            }


            return cmd;
}

