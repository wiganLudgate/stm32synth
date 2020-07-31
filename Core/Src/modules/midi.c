/*
 * midi.c
 *
 *  Created on: 13 juli 2020
 *      Author: Mikael Hessel
 */

#include "modules/midi.h"
#include "modules/keylist.h"
#include "modules/filter.h"

// usb handle

extern USBH_HandleTypeDef hUsbHostFS;
extern MIDI_ApplicationTypeDef Appli_state;  // replace with proper application (usb_host.c)

extern note_t *curnote;	// pointer to current note, declared in main
extern envelope_t *curenv;

// for changing parameters
extern uint16_t delaytime;
extern float delayamp;

extern float limiter;

// dor selecting filter.
extern float* filterpointer;

// keylist test--------------
keylist_t kl = (keylist_t){0x00, (node_t*)NULL};
// keypress_t *keypress;

// Initialize midi (and usb?)
void initMidi(){
	// Midi buffer clear
	for(int i = 0; i< MIDI_BUF_SIZE; i++){
		midiRxBuf[i] = 0;
	}

}



// Interpret messages in midi buffer
// using globals curnote and curenv. maybe pass as pointers in future?
void parseMidi()
{
	// these two are for monophonic playback
	// static uint8_t lastnote = 0; //remember one note
	// static uint8_t thisnote = 0;

	uint16_t notes;
	uint8_t *ptr = midiRxBuf;
	notes = USBH_MIDI_GetLastReceivedDataSize(&hUsbHostFS) >> 2;  // number of bytes / 4 gives number of messages.
	while (notes --){
		// 0 byte is "cable"
		// 1 byte is message type
		// 2 byte is note in case of note on
		// 3 byte is velocity in case of note on

		// message:
		// lower 4 bits are channel so ignore
		// top 4 bits are Message, 8 is note off and 9 is note on.
		// https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message

		uint32_t cable = *ptr++;
		uint32_t mtype = *ptr++;
		uint32_t mnote = *ptr++;
		uint32_t mvel = *ptr++;
		if((mtype & 0xf0) == 0x90 && mnote < 128){	// note on event
			// curnote->note = mnote; // maybe not needed now?
			// curnote->freq = noteToFreq(mnote);
			// lastnote = thisnote;
			// thisnote = mnote;
			// curnote->amp = mvel/127.0;
			// curenv->phase = ATTACK;
			// curenv->counter = 0;

			// test keylist---------
			addKey(&kl, mnote,mvel);


		}else if((mtype & 0xf0) == 0x80){ // note off event
			/*
			if(mnote == thisnote){
				if (lastnote != 0){
					curnote->f = noteToFreq(lastnote);
					thisnote = lastnote;
					lastnote = 0;
					HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin); // Toggle .
				}else{
					// BUG, this does not work until lastnote is cleared once.
					curenv->phase = RELEASE;
					curenv->counter = 0;
					thisnote = 0;
					HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin); // Toggle .
				}

			}else if(mnote == lastnote){
				lastnote = 0;
			}else {
				// release other button
			}
			 */
			// test keylist-----------------
			removeKey(&kl, mnote);
		}

		// handle knobs on MPK mini
		if(mtype == 176)
			switch(mnote){

			// hardcoded to first knob on MPK mini
			// change instrument
			case 0x01:
				switch (mvel/31){
				case 0:
					curnote->osc = SINUS;
					break;
				case 1:
					curnote->osc = SAWTOOTH;
					break;
				case 2:
					curnote->osc = SQUARE;
					break;
				case 3:
					curnote->osc = TRIANGLE;
					break;
				default:
					curnote->osc = NOISE;
					break;
				}
				break;

			// use second knob to set attack envelope time (16-bit)
			case 0x02:
				curenv->attack = mvel*8;
				break;

			// use third knob to set release envelope time (16-bit)
			case 0x03:
				curenv->release = mvel*8;
				break;

			case 0x04:
				limiter = mvel/120.0; // allows overdriving!
				break;

			// first knob on second row sets delay time
			case 0x05:
				delaytime = mvel/127.0 * (DELAYBUFSIZE - 1) ;
				break;

			// second knob on second row sets delay amplitude
			case 0x06:
				delayamp = mvel/127.0;
				break;

			// fourth knob on second row sets filter type
			case 0x08:
				switch (mvel/26){
				case 0:
					filterpointer = NULL;
					break;
				case 1:
					filterpointer = lowpass2;
					break;
				case 2:
					filterpointer = lowpass1;
					break;
				case 3:
					filterpointer = highpass1;
					break;
				case 4:
					filterpointer = highpass2;
					break;
				default:
					filterpointer = NULL;
					break;
				}

				break;
			default:
				break;
		}

		//test
		/*
		if(lastnote){
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
		}else{
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
		}
		*/
	}
}



// for handling midi received messages!
// replaces __weak callback in usbh_MIDI.c

void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost)
{
		//HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin); // Toggle orange.

		// Here is where we parse received MIDI data
		parseMidi();	// send pointer to note/array of notes for now

		// and then we enable the interrupt again
		USBH_MIDI_Receive(&hUsbHostFS, midiRxBuf, MIDI_BUF_SIZE);
}

