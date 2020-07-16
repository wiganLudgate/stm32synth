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
#define BDEPTH			16

#define PI				3.14159f
#define TWOPI			6.28318f
#define FRQ_RATIO 		1.0594630943f
#define A_FOURTH 		440.0f

#define BITLIMIT		((1 << BDEPTH) - 1) // pow(2,BDEPTH) - 1

#define ABUFSIZE		256
// #define UPDATEMS 		(ABUFSIZE/4)/(SRATE/1000)

// for voices
#define MAXVOICES		2


// Oscillator types enumerator
enum osctype{ SINUS, SINUS2, SAWTOOTH, TRIANGLE, SQUARE, NOISE, SILENT};

// Envelope phases
enum envPhase { ATTACK, DECAY, SUSTAIN, RELEASE, NOENV };


// envelope struct
typedef struct {
	enum envPhase phase;
	uint16_t	counter;
	float edt; // rate of change calculated
	float current;
	uint16_t attack;
	uint8_t decay;
	uint8_t sustain;
	uint8_t release;
} envelope_t;

// note struct
typedef struct {
	uint8_t note;
	uint8_t active;
	uint16_t time; // maybe change to phase?
	float	f;
	enum osctype osc;
	float amp;
	// envelope_t env;

} note_t;


// sequencer note struct
typedef struct {
	uint8_t note;
	uint8_t nlength;
	uint8_t osc;
	uint8_t ampl;
	// envelope_t env;
} seqnote_t;

// Sequencer struct
typedef struct {
	uint8_t length;
	uint8_t speed;
	seqnote_t notes[];
} seq_t;



typedef struct {

}voice_t;


note_t *curnote;
envelope_t *curenv;


float playSound(uint8_t note, uint16_t time, float f, uint8_t wave);

float noteToFreq(uint8_t note);

float osc(float f, enum osctype ot, uint16_t t);

void initCS43(I2C_HandleTypeDef* c43i2c);

void startCS43(I2C_HandleTypeDef* c43i2c);

void setVolCS43(I2C_HandleTypeDef* c43i2c, uint8_t vol);


void envelopeCalc(envelope_t *env);

float linearInterpolation(float val1, float val2, float offset);

#endif /* INC_MODULES_SOUND_H_ */
