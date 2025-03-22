/*
 * rylr998.h
 *
 *  Created on: Mar 14, 2025
 *      Author: Tomas Francisco Gonzalez
 */

#ifndef INC_RYLR998_H_
#define INC_RYLR998_H_

#include "stm32l0xx_hal.h"


#define END "\r\n"
#define AT "AT+"
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
	uint8_t networkId;      		//valid range: 3-15, 18(default)
	uint16_t address; 				//0~65535 (default 0)
	uint8_t SF;						//*SF7to SF9 at 125kHz, SF7 to SF10 at 250kHz, and SF7 to SF11 at 500kHz
	uint8_t BW;						// 7: 125 KHz (default) 8: 250 KHz 9: 500 KHz
	uint8_t CR;						//1: Coding Rate 4/5 2 :Coding Rate 4/6  3 :Coding Rate 4/7  4 :Coding Rate 4/8
	uint8_t ProgramedPreamble;      //When NETWORKID=18, The value can be configured to 4~24. Other NETWORKID can only be configured to 12
	uint8_t mode;					//0：Transceiver mode (default). 1：Sleep mode.  2 : Smart receiving power saving mode
	uint32_t rxTime;				//<RX time>=30ms~60000ms, (default 1000)
	uint32_t LowSpeedTime;			//<Low speed time>=30ms~60000ms, (default 1000)
	uint32_t baudRate;				//valid range 300-115200(default)
	uint32_t frequency;				//915000000: 915000000Hz ?				Set the frequency as 868500000Hz. AT+BAND=868500000
	uint8_t memory; 				// saves in flash memory=1
	char password[8];				// 8 chars
	uint8_t CRFOP; 					//22: 22dBm(default) 21: 21dBm 20: 20dBm ... 01: 1dBm 00: 0dBm
}RYLR_config_t;

typedef struct{
	uint16_t id;
	uint8_t byte_count;
	char data[64];    //LoRa suports up to 240 data char
	int8_t rssi;
	uint8_t snr;
}RYLR_RX_data_t;



extern RYLR_RX_data_t rx_packet;



void rylr998_config(RYLR_config_t *config_handler);

//Tx
void rylr998_setAddress(uint8_t address);
void rylr998_networkId(uint8_t networkId);
void rylr998_setParameter(uint8_t SF, uint8_t BW, uint8_t CR, uint8_t ProgramedPreamble);
//void rylr998_reset(void);
void rylr998_mode(uint8_t mode, uint32_t rxTime, uint32_t LowSpeedTime);
//void rylr998_setBaudRate(uint32_t baudRate);
void rylr998_setBand(uint32_t frequency,uint8_t memory);
void rylr998_setCPIN(const char *password);
void rylr998_setCRFOP(uint8_t CRFOP);
void rylr998_FACTORY(void);

//Rx
RYLR_RX_command_t rylr998_prase_reciver(uint8_t *pBuff,uint8_t RX_BUFFER_SIZE);
RYLR_RX_command_t rylr998_ResponseFind(char *rxBuffer);


void rylr998_getCommand(RYLR_RX_command_t cmd,uint8_t *rx_buff,uint8_t RX_BUFFER_SIZE);
void rylr998_sendCommand(const char *cmd);

void rylr998_SetInterruptFlag(void);
uint8_t rylr998_GetInterruptFlag(void);
void rylr998_ClearInterruptFlag(void);


//


#endif /* INC_RYLR998_H_ */
