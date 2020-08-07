/*
 * filter.c
 *
 *	Filtering functions (FIR)
 *
 *  Created on: 23 juli 2020
 *      Author: wigan
 */

#include "modules/filter.h"
#include "modules/sound.h"
#include "modules/wavetable.h" // for sinus

#include <stdlib.h> // malloc
#include <string.h> // memset
#include <math.h> // sin

uint8_t filterEnable;

// naive filter implementation, results in a delay of length for output
// requires buffer for ourpur and coefficients defined elsewhere (these should be same length
// implementation from https://stackoverflow.com/questions/19093294/fir-filter-in-c
float filterFIR(float input, float* coeff){
	// move buffer contents (for a long buffer it would be faster to use circular to avoid this)
	uint8_t p = writeFIRBuffer(input);

	float output = 0.0f;

	for(int i = 0; i < FILTERLENGTH; i++){
		output = output + (readFIRBuffer(p++) * coeff[i]);
	}
	return output;
}

// circular buffer implementation
float readFIRBuffer(uint8_t rpos){
	// static uint8_t rpos = 0;
	if(rpos >= FILTERLENGTH){ rpos = rpos - FILTERLENGTH; }
	return firbuf[rpos];
}

uint8_t writeFIRBuffer(float f){
	static uint8_t wpos = 0;
	if(wpos >= FILTERLENGTH){ wpos = 0; }
	firbuf[wpos] = f;
	return wpos++;
}

void initFIRBuffer(){
	// allocate memory for buffer
	firbuf = malloc(sizeof(float)*FILTERLENGTH);
	// zero init buffer
	for(int i = 0; i < FILTERLENGTH; i++){ firbuf[i] = 0.0f; }
}

// Different fixed filters, taps calculated online:
// https://wirelesslibrary.labs.b-com.com/FIRfilterdesigner/
// these are double precision values so could be converted to float since thats their representation in our program


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




// MOOG FILTER
// implementation of https://github.com/ddiakopoulos/MoogLadders/blob/master/src/MusicDSPModel.h

// This appears to be a bit to slow to calculate and limits voices to about 5 depending on waveform
// crashes engine on new cutoff calculation even when sinus calculation is optimized out.

// local variables for moog filter, only float here because of processor
float resonance;
float inres;
float cutoff;
float stage[4];
float delay[4];
float t1,t2,p,k;

void setMoogCutoff(float c){
	cutoff = 2.0 * c / SRATE;


	p = cutoff * (1.8 - 0.8 * cutoff);
	// k = 2.0 * sin(cutoff * M_PI * 0.5) - 1.0; // anything we could do with our table here or is this ok?
	k = 2.0 * sinetable[(uint16_t)(cutoff * (SINELENGTH / 4))] - 1.0;
	t1 = (1.0 - p) * 1.386249;
	t2 = 12.0 + t1 * t1;

	//setMoogResonance(resonance);
	setMoogResonance(inres);
}

void setMoogCutoffTable(uint16_t c){
	// counts on a uint that indexes correctly into table
	// so c = tablepos * 4;

	p = moogCutoffTable[c];
	k = moogCutoffTable[c+1];
	t1 = moogCutoffTable[c+2];
	t2 = moogCutoffTable[c+3];

	//setMoogResonance(resonance);
	setMoogResonance(inres);
}


void setMoogResonance(float r){
	inres = r;
	resonance = r * (t2 + 6.0 * t1) / (t2 - 6.0 * t1);
}

//12 fp multiplications and 8 addition/subtractions per value
float processMoog(float sample){
	float x = sample - resonance * stage[3];

	// 4 cascaded one pole filters?
	stage[0] = x * p + delay[0] * p - k * stage[0];
	stage[1] = stage[0] * p + delay[1] * p - k * stage[1];
	stage[2] = stage[1] * p + delay[2] * p - k * stage[2];
	stage[3] = stage[2] * p + delay[3] * p - k * stage[3];

	// clip band limited signal - needed for resonance not to go out of bounds and hang filter
	// but this impact performance as well (extra 2 multiplications and one division and one subtraction)
	stage[3] -= (stage[3] * stage[3] * stage[3]) / 6.0;
	//stage[3] = limitAndDistort(stage[3]); // other limiter


	delay[0] = x;
	delay[1] = stage[0];
	delay[2] = stage[1];
	delay[3] = stage[2];

	return stage[3];
}

void initMoog(){
	memset(stage, 0, sizeof(stage));
	memset(delay,0,sizeof(delay));
	setMoogCutoff(1000.0f);
	setMoogResonance(0.1f);
}


// pre-calculated cutoff parameters for 10 settings..
const float moogCutoffTable[] = {
		// 1 of 10, 100.000000 Hz
		0.00814681128f, -0.985752523f, 1.37495553f, 13.8905029f,
		// 2 of 10, 200.000000 Hz
		0.0162607152f, -0.971505821f, 1.36370754f, 13.8596983f,
		// 3 of 10, 300.000000 Hz
		0.024341708f, -0.957260489f, 1.35250533f, 13.8292704f,
		// 4 of 10, 400.000000 Hz
		0.0323897973f, -0.943017364f, 1.34134865f, 13.7992163f,
		// 5 of 10, 500.000000 Hz
		0.0404049754f, -0.928777158f, 1.33023763f, 13.7695322f,
		// 6 of 10, 600.000000 Hz
		0.0483872443f, -0.914540529f, 1.31917226f, 13.7402153f,
		// 7 of 10, 700.000000 Hz
		0.0563366115f, -0.900308251f, 1.30815244f, 13.7112627f,
		// 8 of 10, 800.000000 Hz
		0.064253062f, -0.88608098f, 1.29717827f, 13.6826715f,
		// 9 of 10, 900.000000 Hz
		0.0721366107f, -0.87185955f, 1.28624964f, 13.654438f,
		// 10 of 10, 1000.000000 Hz
		0.0799872503f, -0.857644618f, 1.27536678f, 13.6265602f
};
