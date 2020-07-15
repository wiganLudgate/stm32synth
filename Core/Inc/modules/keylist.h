/*
 * keylist.h
 *
 *  Created on: 15 juli 2020
 *      Author: Mikael Hessel
 */

#ifndef INC_KEYLIST_H_
#define INC_KEYLIST_H_

#include <stdint.h>

typedef struct node node_t;
typedef struct keypress keypress_t;
typedef struct keylist keylist_t;

struct keypress {
	uint8_t note;
	uint8_t velocity;
	// state of engine? envelope, oscillator etc.
};

struct node {
	node_t *next;
	keypress_t *keypress;
};

struct keylist {
	uint8_t size;
	node_t first;
};

void addKey(keylist_t *kl, keypress_t *pressed);
void removeKey(keylist_t *kl, uint8_t note);

#endif /* INC_KEYLIST_H_ */
