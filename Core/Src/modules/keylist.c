/*
 * keylist.c
 *
 *  Created on: 15 juli 2020
 *      Author: Mikael Hessel
 */

#include <stdlib.h>

#include "modules/keylist.h"

/*
node_t* initKeylist(uint16_t size){

}
*/

void addKey(keylist_t *kl, keypress_t *pressed){
	// add first in queue

	// check for maxnodes and handle that (remove last add new first)?

	// create node and copy data
	node_t *current = malloc(sizeof(*current));
	current->keypress = pressed;

	// set pointers correctly
	current->next = &(kl->first);
	kl->first = *current;
	kl->size++;
}

void removeKey(keylist_t *kl, uint8_t note){

	// working node set to first
	node_t *current;
	current = &(kl->first);

	// empty list
	if (&(kl->first) == NULL){ return; }
	// very first element
	if (kl->first.keypress->note == note){
		kl->first = *(kl->first.next);
		free(current->keypress);
		free(current);
		kl->size = 0;
		return;
	}

	while(1){
			// nothing found, exit
			if (current->next == NULL){ return; }

			// note found
			if(current->next->keypress->note == note) {
				node_t *tmp = current->next;
				current->next = current->next->next;
				free(tmp->keypress);
				free(tmp);
				kl->size--;
				return;
			}
			current = current->next;
	}
}
