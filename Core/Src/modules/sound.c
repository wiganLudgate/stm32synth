/*
 * sound.c
 *
 *	Sound generation functions
 *
 *  Created on: 15 juni 2020
 *      Author: Mikael Hessel
 */

#include <stdint.h>
#include <math.h>  // needed for pow function

// plays a note at frequencey frq for dur time (milliseconds)
void playSound(float frq, uint16_t dur)
{

}


// convert a note (enum) to a frequency (use midi note data as starting pont?)
// Midi has 128 notes from 0 (C -2) to 127 (F# 7)
// A 4 is 81

// Faster if this is made as table lookup!

float noteToFreq(uint8_t note)
{
	// Twelfth root of 2 as ratio
	// A-4 as starting frequency

	float FRQ_RATIO = 1.059463094359;
	uint16_t A_FOURTH = 440; // better as float?

	return A_FOURTH * pow(FRQ_RATIO, note - 81) ; // 81 is midi note value of A 4


}
