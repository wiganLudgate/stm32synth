/*
 * filter.h
 *
 *	Trying FIR-filtering
 *	and also moog filter
 *
 *  Created on: 23 juli 2020
 *      Author: Mikael Hessel
 */

#ifndef INC_MODULES_FILTER_H_
#define INC_MODULES_FILTER_H_

#include <stdint.h> // int types

// Needs array of filter values (calculate online or with mathematica)
// circular buffer for latest output from synth?

// so that result of synth goes in to circular buffer and what comes out is what is played?

#define FILTERLENGTH 13

extern const float lowpass1[];
extern const float lowpass2[];
extern const float highpass1[];
extern const float highpass2[];
// extern float lowpass4P[];

extern uint8_t filterEnable;

float* firbuf;

void initFIRBuffer();

float filterFIR(float input, float* coeff);

// local functions
uint8_t writeFIRBuffer(float f);

float readFIRBuffer(uint8_t rpos);


/* MOOG filter ?
 * set frequency
 * set resonance
 * calculate filter
 * ?
 * filter()
 */

// implementation of https://github.com/ddiakopoulos/MoogLadders/blob/master/src/MusicDSPModel.h
void setMoogCutoff(float c);
void setMoogResonance(float r);
void initMoog();
float processMoog(float f);

void setMoogCutoffTable(uint16_t c);
const float moogCutoffTable[40];

#endif /* INC_MODULES_FILTER_H_ */
