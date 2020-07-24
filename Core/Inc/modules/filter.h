/*
 * filter.h
 *
 *	Trying FIR-filtering
 *
 *  Created on: 23 juli 2020
 *      Author: Mikael Hessel
 */

#ifndef INC_MODULES_FILTER_H_
#define INC_MODULES_FILTER_H_

// Needs array of filter values (calculate online or with mathematica)
// circular buffer for latest output from synth?

// so that result of synth goes in to circular buffer and what comes out is what is played?

#define FILTERLENGTH 13

extern const float lowpass1[];
extern const float lowpass2[];
extern const float highpass1[];
extern const float highpass2[];



float filterFIR(float input, float* buffer, float* coeff, uint8_t length);


#endif /* INC_MODULES_FILTER_H_ */
