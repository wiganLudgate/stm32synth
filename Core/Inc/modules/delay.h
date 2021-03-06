/*
 * delay.h
 *
 *  Created on: 9 juli 2020
 *      Author: Mikael Hessel
 */

#ifndef INC_MODULES_DELAY_H_
#define INC_MODULES_DELAY_H_

#include <stdlib.h>
#include <stdint.h>

// takes 4 times size in bytes because floats
// maybe it should be rewritten to use less data (like 16 bit since thats all we output anyway)?
#define DELAYBUFSIZE 8192

typedef struct{
	uint16_t current;
	uint16_t size;
	float data[];
}ringbuf_t;

ringbuf_t* initDelaybuffer(uint16_t size);
void deleteDelaybuffer(ringbuf_t* buffer);

void writeDelay(ringbuf_t* buffer, float f);
float readDelay(ringbuf_t* buffer);
float readDelayOffset(ringbuf_t* buffer, uint16_t offset);

#endif /* INC_MODULES_DELAY_H_ */
