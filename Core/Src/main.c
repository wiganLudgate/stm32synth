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
  *	Polyphonic synthesizer with midi support.
  *	Sinus, Sawtooth, Square, Triangle and Noise waveforms
  *	Delay, some FIR-filters and AR-envelope
  *
  *	Uses USB MIDI Class expansion by: Xavier Halgand
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

#include "modules/keylist.h"

#include "modules/filter.h"



// ----------for testing interrupt
#include <math.h>
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// #define FREQ	 		440.0f		// initial frequency, for test

// #define RUN_TEST

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


// DAC data buffer (ABUFSIZE is actually 4 times the time of an individual buffer due to stereo and half buffer interrupt)
int16_t I2S_data[ABUFSIZE];

// testing delay
ringbuf_t* delaybuf;

extern uint16_t delaytime;
extern float delayamp;

// current note and envelope, from sound.h-----
extern note_t *curnote;
extern envelope_t *curenv;


// MIDI handling
extern MIDI_ApplicationTypeDef Appli_state; // defined in usb_host.c
extern USBH_HandleTypeDef hUsbHostFS;


// test for polyphony
note_t* voices[MAXVOICES];
extern keylist_t kl;


float limiter = 1.0f/MAXVOICES;


// test of filter
const float* filterpointer = NULL;

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

	// ---------------zero fill filterbuffer;
	initFIRBuffer();

	// new filter init;
	initMoog();

	// ---------------create delay buffer
	delaybuf = initDelaybuffer(DELAYBUFSIZE);

	// Allocate memory for current note, this is then used by
	// midi parser and playback
	curnote = malloc(sizeof(*curnote)); // + sizeof(envelope_t));  // is this correct?
	curenv = malloc(sizeof(*curenv));

	// default sound and envelope:
	//
	// note 72, 440 Hz, time 0, Sinus wave, amplitude 1
	// TODO: this might not be needed anymore...
	curnote->note = 72;
	curnote->freq = 440.f;
	curnote->phase = 0;
	curnote->osc = SINUS; // this one is only one used
	curnote->velocity = 127;

	// envelope phase, counter, edt, current env,attack, decay, sustain, release
	// TODO: maybe only keep track of attack and release settings in two variables?
	curenv->phase = ATTACK;
	curenv->attack = 0;
	curenv->decay = 63;
	curenv->sustain = 63; // not needed
	curenv->release = 0;
	// maybe other values should also be initalized?


	// create empty voices and clear necessary data
	for(int i=0; i < MAXVOICES; i++){
		voices[i] = (note_t *)malloc(sizeof(note_t));
		voices[i]->env = (envelope_t *)malloc(sizeof(envelope_t));
		voices[i]->note = 255;
		voices[i]->active = 0;
	}


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
  setVolCS43(&hi2c1, 180);
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

		  // Stop playback by clearing midi-note list
		  removeAllKeys(&kl);
		  break;
	  default:
		  break;
	  }

	  // check if anything in queue to print


	  // small delay seems to be necessary
	  HAL_Delay(1);

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


// Polyphonic playback

// Goes through list of pressed keys and updates state of voices.
// Then adds sound of each voice to next buffer stage.

void playback(uint16_t start, uint16_t stop){
	static float dacdata;

	dacdata = 0.0f;

	int16_t dacwrite = 0;
	node_t *currentnode = kl.first;
	keypress_t kp;

	keypress_t addvoices[MAXVOICES]; // store voices to be added

	// counting variables
	uint8_t num = 0;
	uint8_t index = 0;

	// update active voices

	// 0 not active, 1 ready, 2 fadeout, 3 release, 4 playing

	// set all inactive
	// clear list to add
	for(uint8_t j = 0; j < MAXVOICES; j++){
		// set active to two for all (will go into release mode if inactive, if zero stays that way)
		voices[j]->active = (voices[j]->active) ? 2 : 0;

		// note value of 255 is marker of empty note
		addvoices[j].note = 255;
	}

	// set activate if still playing
	// create new for others
	for(uint8_t j = 0; j < MAXVOICES; j++){
		if (currentnode != NULL){
			readKey(currentnode, &kp);
			currentnode = currentnode->next;

			index = 0;

			// go through voices
			for(uint8_t k = 0; k < MAXVOICES; k++){
				// if voice matches pressed key set it to active again and update
				if (voices[k]->note == kp.note){
					voices[k]->active = 3;
					//update envelope phases here?

					if(voices[k]->env->counter == 0){
						switch(voices[k]->env->phase){
							case ATTACK: // attack phase is over, go into DEACY
								voices[k]->env->phase = DECAY;
								// voices[k]->env->phase = SUSTAIN;
								envelopeCalc(voices[k]->env);
								break;
							case DECAY:
								voices[k]->env->phase = SUSTAIN; // if no decay phase go into sustain
								envelopeCalc(voices[k]->env);
								break;
							case SUSTAIN:
								// do nothing

								break;

								// in case note is in release phase, reactivate with fastfade
							case RELEASE: // this should not happen

								break;
							case FASTFADE:
								// after a fastfade the next note should be queued so go to ATTACK for that
								voices[k]->env->phase = ATTACK;
								// queue note data here
								voices[index]->freq = noteToFreq(voices[index]->note);
								voices[index]->phase = 0.0f;
								voices[index]->osc = curnote->osc;

								envelopeCalc(voices[k]->env);
								break;
							case INACTIVE:
								// do
								break;
							default:
								//voices[k]->env->current = voices[k]->env->sustain/127.0;
								//voices[k]->env->edt = 0.0;
								break;
						}

					}else{
						if(voices[k]->env->phase == RELEASE){
							voices[k]->env->phase = FASTFADE;
							voices[k]->velocity = kp.velocity;
							// TODO: update envelope setting ? or do it in fastdfade
							envelopeCalc(voices[k]->env);
						}
						voices[k]->env->counter--;
					}

					index = 1;
					break; // note found and marked, break inner for-loop
				}
			}
			// index 0 means note was not found already
			if( index  == 0 ){
				// copy keypress to voice data for adding
				addvoices[num].note = kp.note;
				addvoices[num].velocity = kp.velocity;
				// increase num
				num++;
			}
		}else{
			// this means no more nodes so no more keys are pressed
			// break outer for loop (all keys are either already playing or added to queue)
			break;
		}
	}
	// add voice data to inactive voices
	num = 0;
	index = 0;

	while(addvoices[num].note != 255 && num < MAXVOICES ){
		// find non-active voice
		while(voices[index]->active >= 2 && index < MAXVOICES){ index++; }
		// if no more voices  exit
		if(index >= MAXVOICES){ break; }

		// basic setup to do for all
		voices[index]->note = addvoices[num].note;
		voices[index]->velocity = addvoices[num].velocity;

		// if not not active set rest of parameters
		if(voices[index]->active == 0){
			voices[index]->freq = noteToFreq(voices[index]->note);
			voices[index]->phase = 0.0f;
			voices[index]->active = 3;
			voices[index]->osc = curnote->osc; // so maybe store current synth settings somewhere else?
		// also add current settings of envelope here
			voices[index]->env->phase = ATTACK;
			voices[index]->env->current = 0.0;
		}else{
			// if note is in release mode queue for FASTFADE
			voices[index]->env->phase = FASTFADE;
		}


		voices[index]->env->attack = curenv->attack;
		voices[index]->env->decay = curenv->decay;
		voices[index]->env->sustain = addvoices[num].velocity;
		voices[index]->env->release = curenv->release;
		envelopeCalc(voices[index]->env);
		num++;
	}

	// handle release envelope and
	// erase non playing notes
	for(int i = 0; i<MAXVOICES; i++){
		if (voices[i]->active == 2){
			if(voices[i]->env->phase == RELEASE){
				if(voices[i]->env->counter == 0){
					voices[i]->active = 0; // turn off voice, release is finished
					voices[i]->note = 254; // set note value so it won't retrigger
				}else{
					voices[i]->env->counter--;
				}
			}else{ // note is set to go to release phase
				voices[i]->env->phase = RELEASE;
				envelopeCalc(voices[i]->env);
			}

		}
	}



	// iterate over buffer steps
	for(uint16_t i = start; i < stop; i++){

		// iterate over active voices
		for(int j = 0; j < MAXVOICES; j++){

			if(voices[j]->active != 0){

				// Voice active
				// Calculate parameters
				voices[j]->phaseinc = voices[j]->freq / SRATE;
				voices[j]->numsamp = SRATE / voices[j]->freq;

				// update voice envelope here
				voices[j]->env->current += voices[j]->env->edt;

				// Add data to output
				//dacdata += (voices[j]->env->current * playSound( voices[j] ));
				dacdata += ((voices[j]->env->current) * playSound( voices[j] ));
				// (voices[j]->env->current);
			}
		}
		// total volume depends on maximum number of voices (or set limit)..
		dacdata *= limiter;

		  // ---- testing delay
		  // could this be written as ONE function instead?
		  dacdata = (dacdata + readDelayOffset(delaybuf, delaytime) * delayamp) / (1+delayamp);
		  writeDelay(delaybuf, dacdata);

		  // test of filter
		  // uses some preset settings
		  // takes up a bit of cpu time and will limit number of voices... making playback crash
		  /*
		  if (filterpointer != NULL){
			  dacdata = filterFIR(dacdata, filterpointer);
		  }
		   */

		  // new filter
		  // probably should filter whole buffer at a time to avoid function call overhead
		  dacdata = processMoog(dacdata);


		  // hard clip if over range and distort
		  dacdata = limitAndDistort(dacdata);

		// write data to buffer (stereo so same data on both channels)
		dacwrite = dacdata * (BITLIMIT/2 - 1);
		I2S_data[i*2] = dacwrite;
		I2S_data[i*2 + 1] = dacwrite;
	}
}

// runs when buffer half empty
// fill first half of buffer
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  playback(0, ABUFSIZE/4);

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

  playback(ABUFSIZE/4, ABUFSIZE/2);

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
