/*
 * filter.c
 *
 *	Filtering functions (FIR)
 *
 *  Created on: 23 juli 2020
 *      Author: wigan
 */

#include <stdint.h> // int types
// #include <stdlib.h> // malloc


// naive filter implementation, results in a delay of length for output
// requires buffer for ourpur and coefficients defined elsewhere (these should be same length
// implementation from https://stackoverflow.com/questions/19093294/fir-filter-in-c
float filterFIR(float input, float* buffer, float* coeff, uint8_t length){
	// move buffer contents (for a long buffer it would be faster to use circular to avoid this)
	for(int i = (length - 1); i > 0; i--){
		buffer[i] = buffer[i-1];
	}
	buffer[0] = input;

	float output = 0.0f;
	for(int i = 0; i < length; i++){
		output = output + (buffer[i] * coeff[i]);
	}
	return output;
}

// 13 tap lowpass PB 0-1000 (1.1 magnitude, 1 dB ripple), SB 4000-24000 (-30dB)
const float lowpass1[] = {
		0.05223173082571468,0.04198342745248042,0.05463655048033114,0.06624254256664493,
		0.07558084386901394,0.08163953914568195,0.08373570121320612,0.08163953914568195,
		0.07558084386901394,0.06624254256664493,0.05463655048033114,0.04198342745248042,
		0.05223173082571468
};

// 13 tap lowpass PB 0-1000 (1.1 magnitude, 1 dB ripple), SB 8000-24000 (-30dB)
const float lowpass2[] = {
		0.0023392877835954886,0.020042628450998197,0.0461840414317565,0.08374702805950382,
		0.1231730419184006,0.1535076983418161,0.16491210227314534,0.1535076983418161,
		0.1231730419184006,0.08374702805950382,0.0461840414317565,0.020042628450998197,
		0.0023392877835954886
};

// 13 tap highpass SB 0-1000 (-30dB), PB 4000-24000 (1.1 magnitude, 1 dB ripple)
const float highpass1[] = {
		0.002224473531300658,-0.10417256233250581,-0.07057213280789247,-0.08586584039875843,
		-0.09910465406653922,-0.10772011867265581,0.8892568000117616,-0.10772011867265581,
		-0.09910465406653922,-0.08586584039875843,-0.07057213280789247,-0.10417256233250581,
		0.002224473531300658
};


// 13 tap highpass SB 0-1000 (-30dB), PB 8000-24000 (1.1 magnitude, 1 dB ripple)
const float highpass2[] = {
		0.0214432590853922,0.004088406544000778,-0.025226639028456635,-0.0766347033392342,
		-0.13752764671293524,-0.1871703765294358,0.7936802330287256,-0.1871703765294358,
		-0.13752764671293524,-0.0766347033392342,-0.025226639028456635,0.004088406544000778,
		0.0214432590853922
};
