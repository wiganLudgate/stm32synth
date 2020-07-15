/*
 * test.c
 *
 *  Created on: 11 juni 2020
 *      Author: Mikael Hessel
 */

#include "modules/test/test.h"
#include "modules/test/test_serial.h"

void test_all()
{
	test_serial_all();
}

/* TEST CODE FOR OSCILLATOR
// Play some notes (C4 to C5)

if(note++ >= 85){
	note = 72;
	if(wave++ >=4){ wave = 0; }
}
f = noteToFreq(note);

dt = f/SRATE;

// wait half a second
HAL_Delay(200);
 	*/
