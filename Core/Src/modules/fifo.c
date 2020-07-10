/*
 * fifo.c
 *
 *  Created on: 9 juli 2020
 *      Author: Mikael Hessel
 */


// maybe this should all be a circular buffer instead? (for DSP operations).

// unsure if this works or will this memory not be allocated correctly after function finishes?

/*

fifo_t* initFIFO(size_t size){
	fifo_t *fifo = malloc(sizeof(fifo_t));
	fifo->head = 0; // point to first element of buffer
	fifo->tail = 0; // point to last element of buffer
	fifo->size = size; // size of buffer
	fifo->data = malloc(sizeof(float*) * size); // data points
	return fifo;
}

// same goes here
void deleteFIFO(fifo_t* fifo){
	free(fifo->data);
	free(fifo);
}

uint8_t pushFIFO(fifo_t* fifo, float f){
	// if(fifo->head == fifo->tail){ return NULL; } // buffer full! (or just initialized?)
	// if (fifo->tail ==


	// is buffer full, throw error of some kind

	// is buffer wrapping, correct pointer
	// update pointer and store data
	// exit with success code
}

float popFIFO(){
	// is buffer empty
	return f;
}

*/
