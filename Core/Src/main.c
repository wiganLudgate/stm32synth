/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  *	STM 32 Synthesizer
  *	by Mikael Hessel
  *	summer of 2020
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// #include <stdint.h>


#include "modules/test/test.h"
#include "modules/sound.h"
#include "modules/delay.h"
#include "modules/midi.h"

// ----------for testing interrupt
#include <math.h>
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// ----------for testing interrupt

#define TWOPI			6.28318f
#define SRATE			48000.0f
#define FREQ	 		440.0f		// initial frequency, for test


// #define RUN_TEST

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// just for testing
uint16_t t = 0;

// DAC data buffer (ABUFSIZE is actually 4 times the time of an individual buffer due to stereo and half buffer interrupt)

int16_t I2S_data[ABUFSIZE];

// ----------for testing playback
float f = FREQ;				// current playback frequency

float dt = FREQ/SRATE;		//
float amp = 1;				// initial amplification

uint16_t time;

uint8_t note = 72;			// current note

// testing delay
ringbuf_t* delaybuf;

extern uint16_t delaytime;
extern float delayamp;

// temp for changing waveform
uint8_t wave = SINUS;
//uint8_t wave = SILENT;

uint8_t seqPos;	// current position in sequence;

uint8_t playNote = 0;


// test envelope
// envelope_t env = { NOENV, 0, 0, 10, 0, 0, 0};


// current note and envelope, from sound.h-----
extern note_t *curnote;
extern envelope_t *curenv;


// MIDI handling
extern MIDI_ApplicationTypeDef Appli_state; // defined in usb_host.c
extern USBH_HandleTypeDef hUsbHostFS;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

// --------------- user process callback for status light handling
// static void USBH_UserProcess_callback(USBH_HandleTypeDef *phost, uint8_t id);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	// ---------------create delay buffer
	delaybuf = initDelaybuffer(DELAYBUFSIZE);

	// Allocate memory for current note, this is then used by
	// midi parser and playback
	curnote = malloc(sizeof(*curnote)); // + sizeof(envelope_t));  // is this correct?
	curenv = malloc(sizeof(*curenv));
	// default sound:
	curnote->osc = SINUS;
	// maybe other values should also be initalized?


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
  MX_I2C1_Init();
  MX_I2S3_Init();
  MX_USB_HOST_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // run init of DAC-chip, communicate over i2c handle 1
  initCS43(&hi2c1);
  setVolCS43(&hi2c1, 150);
  startCS43(&hi2c1);

  // send som data to start I2S clock
  // Clear buffer
  for (int i = 0; i < ABUFSIZE; i++){ I2S_data[i] = 0x0000; }
  // Start DMA-transfer
  HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t *)I2S_data, ABUFSIZE);


// Entry point for test functions
#ifdef RUN_TEST

  test_all();

#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
   MX_USB_HOST_Process();
   HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);

    /* USER CODE BEGIN 3 */


   	   // Main application switches on state of usb-connection
	  switch(Appli_state){
	  case APPLICATION_READY:
		  // start receiving midi data
		  // and change state
		  USBH_MIDI_Receive(&hUsbHostFS, midiRxBuf, MIDI_BUF_SIZE);
		  Appli_state = APPLICATION_RUNNING;
		  // f=440;
		  break;
	  case APPLICATION_RUNNING:
		  break;
	  case APPLICATION_DISCONNECT:
		  // stop receiving midi data and silience output
		  USBH_MIDI_Stop(&hUsbHostFS);
		  // playNote=0;
		  f=0;
		  break;
	  default:
		  break;
	  }

	  HAL_Delay(1);
/*
	  if(playNote == 1){

	  }else{

	  }
*/
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 50;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


// this is the main playback routine for now (that calculates waveform and fills buffer)

void forPlay(uint16_t start, uint16_t stop)
{
	  float dacdata = 0.0f;
	  static float e = 1.0f;

	  if(curenv->counter == 0){
		  switch(curenv->phase){
			  case ATTACK:
				  envelopeCalc(curenv);
				  e = 0.0;
				  curenv->phase = NOENV;
				  break;
			  case RELEASE:
				  curenv->current = e;
				  envelopeCalc(curenv);
				  curenv->phase = NOENV;
				  break;
			  default:
				  // e = 1.0;
				  curenv->edt = 0;
				  break;
		  }
	  }else{
		  (curenv->counter)--;
	  }

	  // fill buffer from 0 to half of buffersize
	  for(uint16_t i = start; i < stop; i++){
		  // update envelope
		  if(curenv->counter){
		  	  e += curenv->edt;
		  }

		  // calculate waveform
		  dacdata = (curnote->amp * e * playSound(curnote->note, curnote->time, curnote->f, curnote->osc));

		  // ---- testing delay
		  dacdata = dacdata * (1 - delayamp) + readDelayOffset(delaybuf, delaytime) * delayamp;
		  writeDelay(delaybuf, dacdata);

		  //dacdata = sinf(time * TWOPI * dt );

		  // convert float to signed 16 bit integer (implicit conversion)
		  // both channels
		  I2S_data[i*2] = dacdata * (BITLIMIT/2 - 1);
		  I2S_data[i*2 + 1] = dacdata * (BITLIMIT/2 - 1);

		  (curnote->time)++;
		  if (curnote->time >= SRATE/curnote->f) { curnote->time = 0; }
	  }

	  // indicator to see if function is running
	  // if(t++ >= 100){HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); t=0;}
}



// runs when buffer half empty
// fill first half of buffer
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  forPlay(0, ABUFSIZE/4);

}

/**
  * @brief  Tx Transfer completed callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */

// runs when buffer empty
// fill second half of buffer
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  forPlay(ABUFSIZE/4, ABUFSIZE/2);

}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
