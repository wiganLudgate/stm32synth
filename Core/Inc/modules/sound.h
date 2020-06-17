/*
 * sound.h
 *
 *  Created on: 15 juni 2020
 *      Author: Mikael Hessel
 */

#ifndef INC_MODULES_SOUND_H_
#define INC_MODULES_SOUND_H_

#include "stm32f4xx_hal.h"

#define CS43ADDR 	0x94


void playSound(float frq, uint16_t dur);

float noteToFreq(uint8_t note);


void initCS43(I2C_HandleTypeDef* c43i2c);

void startCS43(I2C_HandleTypeDef* c43i2c);

void setVolCS43(I2C_HandleTypeDef* c43i2c, uint8_t vol);

#endif /* INC_MODULES_SOUND_H_ */
