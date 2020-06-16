/*
 * sound.h
 *
 *  Created on: 15 juni 2020
 *      Author: Mikael Hessel
 */

#ifndef INC_MODULES_SOUND_H_
#define INC_MODULES_SOUND_H_

void playSound(float frq, uint16_t dur);

float noteToFreq(uint8_t note);

#endif /* INC_MODULES_SOUND_H_ */
