/*
 * sound.h
 *
 *  Created on: 15 juni 2020
 *      Author: Mikael Hessel
 */

#ifndef INC_MODULES_SOUND_H_
#define INC_MODULES_SOUND_H_

#include "stm32f4xx_hal.h"
#include "modules/wavetable.h"

#define CS43ADDR 		0x94

// define CS43L22 adresses

#define PASSTHU_VOLA	0x14
#define PASSTHU_VOLB	0x15

#define MASTER_VOLA		0x20
#define MASTER_VOLB		0x21

// put in struct for note info
// an array of those is then a sequence

// put in struct for envelope?


#define SRATE			44100.0f
#define BDEPTH			16

#define PI				3.14159f // USE M_PI instead? but that is a double
#define TWOPI			6.28318f
#define FRQ_RATIO 		1.0594630943f
#define A_FOURTH 		440.0f

#define BITLIMIT		((1 << BDEPTH) - 1) // pow(2,BDEPTH) - 1

#define ABUFSIZE		256
// #define UPDATEMS 		(ABUFSIZE/4)/(SRATE/1000)

// for voices
// #define MAXVOICES		9 // (7 voices hangs with sine function) 10 voices hangs sine playback (table), no more time for midi!
#define MAXVOICES		6 // 6 voices works currently (except with filter)

#define SINETABLEFREQ   (SRATE / SINELENGTH);


// macro
#define	linearInterpolation(val1, val2, offset)		(val1 + ((val2 - val1) * offset))
#define limitAndDistort(in)							( (in >= 1.25) ? 0.984375f : ( (in <= -1.25) ? -0.984375f : (1.1 * in - 0.2 * in * in * in) ) )

// Oscillator types enumerator
enum osctype{ SINUS, SINUS2, SAWTOOTH, TRIANGLE, SQUARE, NOISE, SILENT};

// Envelope phases
enum envPhase { ATTACK, DECAY, SUSTAIN, RELEASE, FASTFADE, INACTIVE };


// envelope struct
typedef struct {
	enum envPhase phase;
	uint16_t	counter;
	float edt; // rate of change calculated
	float current; // current envelope value
	uint16_t attack; // time of attack
	uint8_t decay;	// time of decay
	uint8_t sustain; // level of sustain
	uint16_t release; // time of release
} envelope_t;

// note struct
typedef struct {
	uint8_t note;	// midi note to play
	uint8_t velocity;			// velocity of keypress (goes into sustain value)

	uint8_t active; // type of active,  0 not playing
					// 					1 -
					//					2 Release phase
					//					3 playing
					//					4 -
	float phase;	// caclulate where in waveform to get value. ( instead of integer type time)
	float freq;		// frequency (calculated from note
	enum osctype osc;	// chosen oscillator, could be changed to function pointer?

	float phaseinc;	// phase increment (freq/SRATE) - calculate once per buffer
	float numsamp;	// samples per wavelength (SRATE/freq) - calculate once per buffer

	envelope_t *env;  // active envelope
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
	// unused for now
}voice_t;

note_t *curnote;	// stores current settings for note
envelope_t *curenv;	// stores current settings for envelope


float playSound(note_t* n);

float noteToFreq(uint8_t note);

// functions for sound chip
void initCS43(I2C_HandleTypeDef* c43i2c);

void startCS43(I2C_HandleTypeDef* c43i2c);

void setVolCS43(I2C_HandleTypeDef* c43i2c, uint8_t vol);


void envelopeCalc(envelope_t *env);

// float linearInterpolation(float val1, float val2, float offset);
// float limitAndDistort(float in);

#endif /* INC_MODULES_SOUND_H_ */
