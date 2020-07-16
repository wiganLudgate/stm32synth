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


// add note to list
// (does not check for duplicates)
void addKey(keylist_t *kl, uint8_t note, uint8_t velocity){
	// add first in queue

	// check for maxnodes and handle that (remove last add new first)?

	// create node and copy data
	node_t *current = (node_t*)malloc(sizeof(*current));
	// current->keypress = pressed;
	current->note = note;
	current->velocity = velocity;

	// set pointers correctly
	current->next = kl->first;
	kl->first = current;
	kl->size++;
}


// delete node with note from list
void removeKey(keylist_t *kl, uint8_t note){

	// working node set to first
	node_t *current;
	current = kl->first;

	// empty list
	if (kl->first == NULL){ return; }
	// very first element
	if (kl->first->note == note){
		kl->first = kl->first->next;
		//free(current->keypress);
		free(current);
		kl->size = 0;
		return;
	}

	// if nothing found, exit
	while(current->next != NULL){

		// note found
		if(current->next->note == note) {
			node_t *tmp = current->next;
			current->next = current->next->next;
			// free(tmp->keypress);
			free(tmp);
			kl->size--;
			return;
		}
		current = current->next;
	}
}

// read node data into keypress
void readKey(node_t *current, keypress_t *kp){
	if (current != NULL){
		kp->note = current->note;
		kp->velocity = current->velocity;
		current = current->next;
	}
}
