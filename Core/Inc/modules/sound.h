/*
 * sound.h
 *
 *  Created on: 15 juni 2020
 *      Author: Mikael Hessel
 */

#ifndef INC_MODULES_SOUND_H_
#define INC_MODULES_SOUND_H_

#include "stm32f4xx_hal.h"

#define CS43ADDR 		0x94

// define CS43L22 adresses

#define PASSTHU_VOLA	0x14
#define PASSTHU_VOLB	0x15

#define MASTER_VOLA		0x20
#define MASTER_VOLB		0x21

// put in struct for note info
// an array of those is then a sequence

// put in struct for envelope?


#define SRATE			48000.0f
#define BDEPTH			12

#define PI				3.14159f
#define TWOPI			6.28318f
#define FRQ_RATIO 		1.0594630943f
#define A_FOURTH 		440.0f

#define BITLIMIT		((1 << BDEPTH) - 1) // pow(2,BDEPTH) - 1

// Oscillator types enumerator
enum osctype{ SINUS, SAWTOOTH, TRIANGLE, SQUARE, NOISE};

// structure types for a sequencer

// Note struct
typedef struct Notes{
	uint8_t note;
	// uint8_t length;
	uint8_t osc;
	uint8_t ampl;
} note_t;

// Sequencer struct
typedef struct Sequences{
	uint8_t length;
	uint8_t speed;
	note_t *notes;
} seq_t;


uint32_t playSound(uint8_t note, uint16_t time, float f, uint8_t wave);

float noteToFreq(uint8_t note);

uint16_t osc(float f, enum osctype ot, uint16_t t);

void initCS43(I2C_HandleTypeDef* c43i2c);

void startCS43(I2C_HandleTypeDef* c43i2c);

void setVolCS43(I2C_HandleTypeDef* c43i2c, uint8_t vol);

#endif /* INC_MODULES_SOUND_H_ */
