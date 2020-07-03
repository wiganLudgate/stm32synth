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

// ----------for testing interrupt
#include <math.h>
#include <stdlib.h>


// ----------testing USB and midi
#include "external/usbh_MIDI.h"
#include "usbh_def.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// ----------for testing interrupt

#define TWOPI			6.28318f
#define SRATE			48000.0f
#define FREQ	 		440.0f		// for test only

#define ABUFSIZE		256

#define MIDI_BUF_SIZE	64
// #define RUN_TEST

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// just for testing
uint16_t t = 0;

// DAC data

int16_t I2S_data[ABUFSIZE];

// ----------for testing interrupt
float f = FREQ;

float dt = FREQ/SRATE;
float amp = 1;

uint16_t time;

uint8_t note = 72;

// temp for chaning waveform
uint8_t wave = SINUS;
//uint8_t wave = SILENT;

uint8_t seqPos;	// current position in sequence;

// for midi buffer
uint8_t midiRxBuf[MIDI_BUF_SIZE];

uint8_t playNote = 0;

// usb handle

extern USBH_HandleTypeDef hUsbHostFS;
extern MIDI_ApplicationTypeDef Appli_state;  // replace with proper application


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */


void parseMidi();
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
	// When the saints...
	seq_t *mysong = malloc(sizeof *mysong + 4 * sizeof *mysong->notes);
	mysong->length = 32;
	mysong->speed = 50; // for now this will be used for HAL_Delay(10000/speed);
	note_t notes[32] = { {0,0,SILENT,0},
						{72,0,SINUS,200},
						{76,0,SINUS,200},
						{77,0,SINUS,200},
						{79,0,SINUS,200},
						{79,0,SINUS,210},
						{79,0,SINUS,220},
						{79,0,SINUS,230},
						{0,0,SILENT,0},
						{72,0,TRIANGLE,127},
						{76,0,TRIANGLE,127},
						{77,0,TRIANGLE,127},
						{79,0,TRIANGLE,127},
						{79,0,TRIANGLE,140},
						{79,0,TRIANGLE,160},
						{79,0,TRIANGLE,180},
						{0,0,SILENT,0},
						{48,0,SAWTOOTH,127},
						{52,0,SAWTOOTH,127},
						{53,0,SAWTOOTH,127},
						{55,0,SAWTOOTH,127},
						{55,0,SAWTOOTH,160},
						{52,0,SAWTOOTH,127},
						{52,0,SAWTOOTH,160},
						{48,0,SQUARE,127},
						{48,0,SQUARE,127},
						{52,0,SQUARE,127},
						{52,0,SQUARE,127},
						{50,0,SQUARE,127},
						{50,0,SQUARE,140},
						{50,0,SQUARE,150},
						{50,0,SQUARE,160},
						};

	/*

	// init sequence of 8 notes
	seq_t *mysong = malloc(sizeof *mysong + 4 * sizeof *mysong->notes);
	mysong->length = 8;
	mysong->speed = 50; // for now this will be used for HAL_Delay(10000/speed);
	note_t notes[8] = { {81,0,SINUS,255},
						{83,0,SAWTOOTH,127},
						{85,0,SQUARE,180},
						{0,0,SILENT,0},
						{72,0,SAWTOOTH,100},
						{74,0,SAWTOOTH,200},
						{76,0,SAWTOOTH,140},
						{78,0,SAWTOOTH,160}
						};

	 */

	// fill song buffer
	for(int i=0; i<=mysong->length; i++){ mysong->notes[i] = notes[i]; }



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

/*
  // start DAC 1 channel 1
  HAL_DAC_Start(&hdac, DAC1_CHANNEL_1);

  // start timer interrupt TIM2
  HAL_TIM_Base_Start_IT(&htim2);
*/


  // run init of DAC-chip
  initCS43(&hi2c1);
  setVolCS43(&hi2c1, 150);
  startCS43(&hi2c1);

  // send som data to start I2S clock
  // Clear buffer
  for (int i = 0; i < ABUFSIZE; i++){ I2S_data[i] = 0x0000; }

  HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t *)I2S_data, ABUFSIZE);


// Entry point for test functions
#ifdef RUN_TEST

  test_all();

#endif

  // Start USB and set as MIDI device
/*
  if(USBH_Init(&hUSBhost, USBH_UserProcess_callback, 0) != USBH_OK)
	  HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET); // RED LED ON

  if(USBH_RegisterClass(&hUSBhost, USBH_MIDI_CLASS) != USBH_OK)  // deskripator uses an alias here?
	  HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET); // RED LED ON

  if(USBH_Start(&hUSBhost) != USBH_OK)
	  HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET); // RED LED ON
*/



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
   MX_USB_HOST_Process();
   HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);

    /* USER CODE BEGIN 3 */
   /*
   if(USBH_Process(&hUSBhost) != USBH_OK){
   	  HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET);  // RED LED ON
    } else {
    	HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }
    */

// DAC playback testcode
/*
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_data);
    if(dac_data++ > 4095){
    	dac_data=0;
    }
*/


	  /* TEST CODE FOR OSCILLATOR
    // Play some notes (C4 to C5)

    if(note++ >= 85){
    	note = 72;
    	if(wave++ >=4){ wave = 0; }
    }
    f = noteToFreq(note);

    dt = f/SRATE;

    // wait half a second
    HAL_Delay(200);
	   	*/


	  // test for sequence playback;
/*
	  if(seqPos >= mysong->length){
		  seqPos = 0;
	    	}
	  note = mysong->notes[seqPos].note;
	  wave = mysong->notes[seqPos].osc;
	  f = noteToFreq(note);
	  amp = mysong->notes[seqPos].ampl / 255.0;
	  HAL_Delay(10000 / mysong->speed);
	  seqPos++;

*/
	  switch(Appli_state){
	  case APPLICATION_READY:
		  USBH_MIDI_Receive(&hUsbHostFS, midiRxBuf, MIDI_BUF_SIZE);
		  Appli_state = APPLICATION_RUNNING;
		  // f=440;
		  break;
	  case APPLICATION_RUNNING:
		  break;
	  case APPLICATION_DISCONNECT:
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

void parseMidi()
{
	uint16_t notes;
	uint8_t *ptr = midiRxBuf;
	notes = USBH_MIDI_GetLastReceivedDataSize(&hUsbHostFS) >> 2;  // number of bytes / 4 gives number of messages.
	while (notes --){
		// 0 byte is "cable"
		// 1 byte is message type
		// 2 byte is note in case of note on
		// 3 byte is velocity in case of note on

		// message:
		// lower 4 bits are channel so ignore
		// top 4 bits are Message, 8 is note off and 9 is note on.
		// https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message

		uint32_t cable = *ptr++;
		uint32_t mtype = *ptr++;
		uint32_t mnote = *ptr++;
		uint32_t mvel = *ptr++;
		if((mtype & 0xf0) == 0x90){
			f = noteToFreq(mnote);
			amp = mvel/127.0;
		}else if((mtype & 0xf0) == 0x80){
			amp = 0;
		}

		// hardcoded to first knob on MPK mini
		// change i
		if(mtype == 176){
			switch (mvel/31){
			case 0:
				wave = SINUS;
				break;
			case 1:
				wave = SAWTOOTH;
				break;
			case 2:
				wave = SQUARE;
				break;
			case 3:
				wave = TRIANGLE;
				break;
			default:
				wave = NOISE;
				break;
			}
		}

	}
}

// Timer period elapsed callback for interrupt-driven sound generation
// will run at 48 KHz if timer is set up correctly
/* Timer interrupt function for internal dac.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Prevent unused argument(s) compilation warning
  UNUSED(htim);

  if(htim->Instance == TIM2){
	  dac_data = amp * playSound(note, time, f, wave);
	  // dac_data = ((sinf(time * TWOPI * dt ) + 1) * 2047);
	  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_data);

	  // increment time index and reset if necessary
	  time++;
	  if (time >= SRATE/f) { time = 0; }

  }

}
*/

// USB Host User Callback (for lighting some status lights on different states)
/*
static void USBH_UserProcess_callback(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
	  Appli_state = APPLICATION_DISCONNECT;
	  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET); // Green led off
	  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET); // - led off
	  break;
  case HOST_USER_CLASS_ACTIVE:
	  Appli_state = APPLICATION_READY;
	  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET); // Green led off
	  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET); // - led off
	  HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET); // - led off
	  break;
  case HOST_USER_CONNECTION:
	  Appli_state = APPLICATION_START;
	  HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_SET); // - led off
	  break;
  default:
	  break;
  }
}
*/
void forPlay(uint16_t start, uint16_t stop)
{
	  float dacdata = 0.0f;
	  // fill buffer from 0 to half of buffersize
	  for(uint16_t i = start; i < stop; i++){
		  dacdata = (amp * playSound(note, time, f, wave));
		  //dacdata = sinf(time * TWOPI * dt );

		  // convert float to signed 16 bit integer (implicit conversion)
		  // both channels
		  I2S_data[i*2] = dacdata * (BITLIMIT/2 - 1);
		  I2S_data[i*2 + 1] = dacdata * (BITLIMIT/2 - 1);

		  time++;
		  if (time >= SRATE/f) { time = 0; }
	  }

	  // indicator to see if function is running
	  // if(t++ >= 100){HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); t=0;}
}


// Envelope test
float envelopeCalc(envelope *env){
	static uint16_t epos = 0;
	static uint16_t updatesMs = (SRATE/1000)/(ABUFSIZE/4);  // define this instead?
	static float edt = 1.0f;
	// calculate change by getting closest
	switch (env->phase){
		case ATTACK:
			break;
		case DECAY:
			break;
		case SUSTAIN:
			break;
		case RELEASE:
			break;
		case NOENV:
			break;
	}
	return epos++ * edt;

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

// for handling midi received messages!
void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost)
{
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin); // Toggle orange.
		parseMidi();
		USBH_MIDI_Receive(&hUsbHostFS, midiRxBuf, MIDI_BUF_SIZE);
		// USBH_MIDI_Receive(&hUsbHostFS, midiRxBuf, MIDI_BUF_SIZE);
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
