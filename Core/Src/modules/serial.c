/*
 * serial.c
 *
 *  Created on: Jun 10, 2020
 *      Author: Mikael Hessel
 *
 *
 */
#include <stdint.h>

#include "usart.h"
#include "modules/serial.h"


void serialprint(char* str)
{
	uint8_t l = 0; // max 255 chars in string
	while (str[l] != 0 && l < 256){ l++; }

	HAL_UART_Transmit(SERIALPORT, (uint8_t*) str, l, TXTIMEOUT);
}

void serialprintIT(char* str, uint8_t len){
	//HAL_UART_Transmit_IT(SERIALPORT, (uint8_t*)str, len);
	HAL_UART_Transmit_DMA(SERIALPORT, (uint8_t*)str, len);
}
