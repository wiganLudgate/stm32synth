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

typedef struct{
	uint16_t current;
	uint16_t size;
	float data[];
}ringbuf_t;



#endif /* INC_MODULES_DELAY_H_ */
