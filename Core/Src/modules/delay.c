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


ringbuf_t* initDelaybuffer(uint16_t size){
	ringbuf_t *buffer = malloc(sizeof(*buffer) + (sizeof(float) * size));
	buffer->current = 1; // point to current element of buffer
	buffer->size = size; // size of buffer

	// zero fill buffer
	for(uint16_t i = 0; i < size; i++){ buffer->data[i] = 0.f; }
	return buffer;
}


// same goes here
void deleteDelaybuffer(ringbuf_t* buffer){
	free(buffer);
}

/*
 // Handle wraparound.

uint8_t writeDelay(ringbuf_t* buffer, float f){
	// if(fifo->head == fifo->tail){ return NULL; } // buffer full! (or just initialized?)
	// if (fifo->tail ==


	// is buffer full, throw error of some kind

	// is buffer wrapping, correct pointer
	// update pointer and store data
	// exit with success code
}

float readDelay(ringbuf_t* buffer){
	float f = 0.f;
	if (buffer != NULL){
		if(current == 0){ f = buffer->data[buffer->size - 1];
		}else{ f = buffer->data[buffer->current -1]; }

	return f;
}

*/
