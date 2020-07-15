/*
 * sound.c
 *
 *	Sound generation functions
 *
 *	Also functions for controlling CS43L22 DAC chip
 *
 *  Created on: 15 juni 2020
 *      Author: Mikael Hessel
 */

#include "modules/sound.h"

#include "modules/frqtable.h"

#include "i2s.h"

#include <stdint.h>
#include <math.h>  // needed for pow function

// plays a note at frequencey frq for dur time (milliseconds)
float playSound(uint8_t note, uint16_t time, float f, enum osctype wave)
{
	float r;
	//float frq = noteToFreq(note); // too slow, use lookup instead?
	r = osc(f, wave, time);
	return r;
}



// rewritten for returning a float between -1 and 1 for all oscillators

float osc(float f, enum osctype ot, uint16_t time){
	float dt = f/SRATE;

	float retval = 0;
	static uint32_t noise = 22222;

	switch(ot){
	case SINUS:
		// scale result to 12 bit
		// int scaled:
		// retval = ((sinf(time * TWOPI * dt) + 1) * (BITLIMIT/2));
		retval = sinf(time * TWOPI * dt);
		break;
	case SAWTOOTH:
		retval = (time * dt * 2)  - 1;
		break;
	case TRIANGLE:
		retval = fabs(time * dt * 4 - 2) - 1;
		break;
	case SQUARE:
		retval = ( (time < (SRATE/f) / 2) ? -1 : 1);
		break;
	case NOISE:
		// pseudorandom generator
		// https://www.musicdsp.org/en/latest/Synthesis/59-pseudo-random-generator.html
		noise = (noise * 196314165) + 907633515;
		retval = ((noise>>(32-BDEPTH)) / (float)(BITLIMIT/2)) - 1;
		break;
	case SILENT:
		;
	default:
		break;
	}
	return retval;
}

// convert a note (enum) to a frequency (use midi note data as starting pont?)
// Midi has 128 notes from 0 (C -2) to 127 (F# 7)
// A 4 is 81

// Faster if this is made as table lookup!

float noteToFreq(uint8_t note)
{
	// New method - from table
	if(note >= FRQLOWNOTE && note <= FRQHIGHNOTE){
		return notefreq[note - FRQLOWNOTE];
	}else{ return 0; }

	// old method, direct calculation:
	// Twelfth root of 2 as ratio
	// A-4 as starting frequency
	//return A_FOURTH * pow(FRQ_RATIO, note - 81) ; // 81 is midi note value of A 4
}


// set up CS43L22 for analog passthrough
// with som help from: https://github.com/MYaqoobEmbedded
void initCS43(I2C_HandleTypeDef* c43i2c)
{
	// temp variable for sending and receving data
	uint8_t tempdata = 0x00;

	// Enable I2S3
	  __HAL_UNLOCK(&hi2s3);
	  __HAL_I2S_ENABLE(&hi2s3);

	  // set up CS43L22 for output from internal DAC (PA4)
	  // CS43ADDR  write, 0x95 read

	  // Bring reset high to enable chip
	  HAL_GPIO_WritePin(Audio_RST_GPIO_Port, Audio_RST_Pin, GPIO_PIN_SET);

	  HAL_Delay(5); // Short delay after pulling reset high

	  // ( MCLK is enabled by starting I2S transfer in main)

	  // Ensure power down mode (set 0x02 to 0x01)
	  tempdata = 0x01;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x02, 1, &tempdata, 1, 50);

	  // Power control 2, 0x04
	  // Set headphones and speakers on
	  // bit 7,6(B) and 5,4(A) set to 10 for headphone channel on
	  // bit 3,2(B) and 1,0(A) set to 11 for speaker channel off

	  tempdata = 0xAF;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x04, 1, &tempdata, 1, 50);

	  // Clocking control, 0x05
	  // Bit 7 set to 1 for auto
	  tempdata = 0x80;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x05, 1, &tempdata, 1, 50);



	  // Setup as Interface 1 Control (0x06)
	  // Slave mode, SCLK non inveted, DSP disabled, DAC I2S up to 24 bit, AWL 16-bit
	  // bit 0 and 1 are data length (11 is 16 bit)
	  // bit 3 and 2 is DAC format (01 is I2S 24-bit)
	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x06, 1, &tempdata, 1, 50);
	  tempdata &= 0x20; // clear all but reserved bit
	  tempdata |= 0x07; // set up to 24 bit data and 16 bit data length
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x06, 1, &tempdata, 1, 50);


	  // set registers 0x08 and 0x09 (analog passthrough A and B) to AN1x
	  // Not needed for I2S-mode
	  /*
	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x06, 1, &tempdata, 1, 50);
	  tempdata &= 0xF0;
	  tempdata |= 0x01;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x08, 1, &tempdata, 1, 50);

	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x06, 1, &tempdata, 1, 50);
	  tempdata &= 0xF0;
	  tempdata |= 0x01;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x09, 1, &tempdata, 1, 50);
	  */

	  // 0x0E bits 7 and 6 are analog passthrough enable
	  // bits 5 and 4 are PASSMUTE and should be 0
	  // set passthrought bits (first read register, then OR the bits and write back)
	  // bit 1 is digital soft ramp
	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x0E, 1, &tempdata, 1, 50);

	  /* this block enables analog passthrough
	  tempdata |= 0xC0;
	  tempdata &= 0xCF;
	  */
	  tempdata |= 0x02;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x0E, 1, &tempdata, 1, 50);
/*
	  // 0x14 and 0x15 are passthrough volume, 0x00 is 0dB
	  tempdata = 0x00;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x14, 1, &tempdata, 1, 50);
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x15, 1, &tempdata, 1, 50);
	  */

	  // enable headphones and other outputs (0x0F
	  tempdata = 0x00;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x0F, 1, &tempdata, 1, 50);

	  // write pcm volume (1A, 1B)
	  tempdata = 0x00;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x1A, 1, &tempdata, 1, 50);
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x1B, 1, &tempdata, 1, 50);
}

// start CS43L22 with init sequence
void startCS43(I2C_HandleTypeDef* c43i2c)
{
	// temp variable for sending and receving data
	uint8_t tempdata = 0x00;

	  // do init sequence
	  /*
	1. Write 0x99 to register 0x00.
	2. Write 0x80 to register 0x47.
	3. Write ‘1’b to bit 7 in register 0x32.
	4. Write ‘0’b to bit 7 in register 0x32.
	5. Write 0x00 to register 0x00.
	   */
	  tempdata = 0x99;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x00, 1, &tempdata, 1, 50);
	  tempdata = 0x80;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x47, 1, &tempdata, 1, 50);

	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x32, 1, &tempdata, 1, 50);
	  tempdata |= 0x80;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x32, 1, &tempdata, 1, 50);
	  // HAL_I2C_Mem_Read(&hi2c1, CS43ADDR, 0x32, 1, &tempdata, 1, 50);
	  tempdata &= 0x7F;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x32, 1, &tempdata, 1, 50);

	  tempdata = 0x00;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x00, 1, &tempdata, 1, 50);

	 // while(HAL_I2C_IsDeviceReady(&hi2c1, CS43ADDR, 10, 1000) != HAL_OK) {}

	  // Set power ctl 1 (0x02) to 0x9E to do start
	  tempdata = 0x9e;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x02, 1, &tempdata, 1, 50);
}

void stopCS43(I2C_HandleTypeDef* c43i2c)
{
	// temp variable for sending data
	uint8_t tempdata = 0x01;

	// write to power ctl 1
	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x02, 1, &tempdata, 1, 50);
}


// Volume setting from  0 to 255
// 0x00 - 0x18 unusable. normal volume at 0xe7
void setVolCS43(I2C_HandleTypeDef* c43i2c, uint8_t vol)
{
	// max digital volume is +12 db at 0x18;
	uint8_t dvol = ((vol> 0x6e)? (vol - 0xe7):(vol+0x19));

	// max passthrough volume is +12db
	uint8_t avol = ((vol > 0x7f)? (vol - 0x80):(vol + 0x80));


	// Master volume at 0x20, 0x21

	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, MASTER_VOLA, 1, &dvol, 1, 50);
	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, MASTER_VOLB, 1, &dvol, 1, 50);

/* Analog passthrough not needed.
	// 0x14 and 0x15 are passthrough volume, 0x00 is 0dB
	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, PASSTHU_VOLA, 1, &avol, 1, 50);
	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, PASSTHU_VOLB, 1, &avol, 1, 50);
*/

}



// Envelope test
void envelopeCalc(envelope_t *env){
	static const float updatesms = (ABUFSIZE/4)/(SRATE/1000);
	// static uint16_t epos = 0
	// static float edt = 1.0f;
	// calculate change by getting closest
	switch (env->phase){
		case ATTACK:
			env->counter = env->attack ? ((env->attack)/updatesms) + 0.5 : 1;
			env->edt = (1.0/((env->counter)*(ABUFSIZE/4)));
			break;
		case DECAY:
			break;
		case SUSTAIN:
			env->edt = 0.0;
			break;
		case RELEASE:
			env->counter = env->release ? ((env->release)/updatesms) + 0.5 : 1;
			env->edt = - (env->current/((env->counter)*(ABUFSIZE/4)));
			break;
		case NOENV:
			env->edt = 0.0;
			break;
	}
	//return env->edt;
}


float linearInterpolation(float val1, float val2, float offset){
	return val1 + ((val2 - val1) * offset);
}


// function to initialise the voices of the synth
void initVoices(){
	// make an array of voice structs
	// malloc for voice structs
	for(int i = 0; i < MAXVOICES; i++){
		// init each voice struct
	}
	// return the array
}


// playback function for the voices
// outputs a sample of the resulting mix
float playVoices(){
	float mix = 0.f; // init returnvalue to 0
	for(int i = 0; i < MAXVOICES; i++){
		float output = 0.f;
		/*
		if (voice->active != 0){

			output = osc(voice->f, voice->osc, voice->phase);
			output *= voice->amp;
			output *= envelopeCalc(&(voice->env));
		}
		*/
		// for each voice calculate output
		mix += output/MAXVOICES;

	}
	// add delay and other effectseffects
	return mix;
}


// for now this will contain old sequencer code
// Should return current sequence note?
void sequenceGet(){

	// old Sequence playback
	// When the saints...
	seq_t *mysong = malloc(sizeof *mysong + 4 * sizeof *mysong->notes);
	mysong->length = 32;
	mysong->speed = 50; // for now this will be used for HAL_Delay(10000/speed);
	seqnote_t notes[32] = { {0,0,SILENT,0},
						{72,0,SINUS,200},
						{76,0,SINUS,200},
						{77,0,SINUS,200},
						{79,0,SINUS,200},
						{79,0,SINUS,210},
						{79,0,SINUS,220},
						{79,0,SINUS,230},
						{0,0,SILENT,0},
						{72,0,TRIANGLE,127},
						{76,0,TRIANGLE,127},
						{77,0,TRIANGLE,127},
						{79,0,TRIANGLE,127},
						{79,0,TRIANGLE,140},
						{79,0,TRIANGLE,160},
						{79,0,TRIANGLE,180},
						{0,0,SILENT,0},
						{48,0,SAWTOOTH,127},
						{52,0,SAWTOOTH,127},
						{53,0,SAWTOOTH,127},
						{55,0,SAWTOOTH,127},
						{55,0,SAWTOOTH,160},
						{52,0,SAWTOOTH,127},
						{52,0,SAWTOOTH,160},
						{48,0,SQUARE,127},
						{48,0,SQUARE,127},
						{52,0,SQUARE,127},
						{52,0,SQUARE,127},
						{50,0,SQUARE,127},
						{50,0,SQUARE,140},
						{50,0,SQUARE,150},
						{50,0,SQUARE,160},
						};

	/*

	// init sequence of 8 notes
	seq_t *mysong = malloc(sizeof *mysong + 4 * sizeof *mysong->notes);
	mysong->length = 8;
	mysong->speed = 50; // for now this will be used for HAL_Delay(10000/speed);
	seqnote_t notes[8] = { {81,0,SINUS,255},
						{83,0,SAWTOOTH,127},
						{85,0,SQUARE,180},
						{0,0,SILENT,0},
						{72,0,SAWTOOTH,100},
						{74,0,SAWTOOTH,200},
						{76,0,SAWTOOTH,140},
						{78,0,SAWTOOTH,160}
						};

	 */

	// fill song buffer
	for(int i=0; i<=mysong->length; i++){ mysong->notes[i] = notes[i]; }

// from main while-loop:
	  // test for sequence playback;
/*
	  if(seqPos >= mysong->length){
		  seqPos = 0;
	    	}
	  note = mysong->notes[seqPos].note;
	  wave = mysong->notes[seqPos].osc;
	  f = noteToFreq(note);
	  amp = mysong->notes[seqPos].ampl / 255.0;
	  HAL_Delay(10000 / mysong->speed);
	  seqPos++;

*/

}
