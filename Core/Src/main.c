/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rylr998.h"
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


#define RX_BUFFER_SIZE 255
uint8_t rx_buff[RX_BUFFER_SIZE];  // Reception buffer



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    // Check if the event is an idle line event

	if((huart == &huart2)){



		//if (HAL_UARTEx_GetRxEventType(huart) == HAL_UART_RXEVENT_IDLE ||HAL_UARTEx_GetRxEventType(huart) == HAL_UART_RXEVENT_HT) {
		//For some reason, the RXevent IDLE tends to not work right
			if (rx_buff[Size-1]=='\n'){
			rylr998_SetInterruptFlag();
			}
			HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_buff, RX_BUFFER_SIZE);


	}

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_buff, RX_BUFFER_SIZE);

  if(rylr998_networkId(&hlpuart1,18)==HAL_OK){
	  while (1){
	 	  if(rylr998_GetInterruptFlag()){
	 		  rylr998_prase_reciver(rx_buff,RX_BUFFER_SIZE);
	 		  break;
	 	  }
	   }
  }
/* Example of usage: Start DMA Rx  -> Send command -> wait for HAL_OK -> look for IRQ flag
	if(!rylr998_networkId(&hlpuart1,18)==HAL_OK){
	}

	HAL_Delay(1000);
	if(rylr998_setAddress(&hlpuart1,1234)==HAL_OK){
	}


	HAL_Delay(1000);
	if(rylr998_setParameter(&hlpuart1,11,7,1,12)==HAL_OK){
	}

	HAL_Delay(1000);
	if(rylr998_reset(&hlpuart1)==HAL_OK){
	}

	HAL_Delay(1000);
	if(rylr998_mode(&hlpuart1,1,59999,50000)==HAL_OK){
	}

	HAL_Delay(1000);
	if(rylr998_setBaudRate(&hlpuart1,115200)==HAL_OK){
	}

	HAL_Delay(1000);
	if(rylr998_setBand(&hlpuart1,915000000,1)==HAL_OK){ //Saves it on flash
	}

	HAL_Delay(1000);
	if(rylr998_setBand(&hlpuart1,915000000,0)==HAL_OK){   // not on MEMORY
	}

	HAL_Delay(1000);
	char password[] = "TOMAS123";
	if(rylr998_setCPIN(&hlpuart1,password)==HAL_OK){
	}

	HAL_Delay(1000);
	if(rylr998_setCRFOP(&hlpuart1,22)==HAL_OK){
	}

	HAL_Delay(1000);
	if(rylr998_FACTORY(&hlpuart1)){
	 }

	HAL_Delay(1000);
	//Example : Send HELLO string to the Address 50, AT+SEND=50,5,HELLO
	uint8_t data_to_send[]= "Hello World, im learning to use LoRa communication";
	if(rylr998_sendData(&hlpuart1,0,(uint8_t*)&data_to_send,strlen((char*)data_to_send))==HAL_OK){
	}

*/

  while (1)
  {
	  if(rylr998_GetInterruptFlag()){
		  rylr998_prase_reciver(rx_buff,RX_BUFFER_SIZE);
	  }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_LPUART1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
