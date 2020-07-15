/*
 * delay.c
 *
 *	ring buffer and delay functions..
 *
 *  Created on: 9 juli 2020
 *      Author: Mikael Hessel
 */


// unsure if this works or will this memory not be allocated correctly after function finishes?

#include "modules/delay.h"


// for to get delay to work for now
uint16_t delaytime;
float delayamp;


ringbuf_t* initDelaybuffer(uint16_t size){
	ringbuf_t *buffer = malloc(sizeof(*buffer) + (sizeof(float) * size));
	buffer->current = 1; // point to current element of buffer
	buffer->size = size; // size of buffer

	// zero fill buffer
	for(uint16_t i = 0; i < size; i++){ buffer->data[i] = 0.f; }


	// init delayvalues
	delaytime = 0;
	delayamp = 0.f;

	return buffer;
}


// same goes here
void deleteDelaybuffer(ringbuf_t* buffer){
	free(buffer);
}


// add item to buffer
void writeDelay(ringbuf_t* buffer, float f){
	// Handle wraparound.
	if(buffer->current >= buffer->size){
		buffer->current = 0;
	}
	buffer->data[buffer->current++] = f;
}

// read item from buffer
float readDelay(ringbuf_t* buffer){
	float f = 0.f;
	//if (buffer != NULL){
		if(buffer->current == buffer->size){
			f = buffer->data[0];
		}else{
			f = buffer->data[buffer->current];
		}
	// }
	return f;
}

// read item from buffer with offset
float readDelayOffset(ringbuf_t* buffer, uint16_t offset){
	float f = 0.f;
	int32_t  index;
	// if offset out of bounds return maximum offset
	if(offset >= buffer->size){ offset = buffer->size - 1;}

	index = buffer->current - offset - 1;

	if(index < 0){
		f = buffer->data[index + buffer->size];
	}else{
		f = buffer->data[index];
	}

	return f;
}
