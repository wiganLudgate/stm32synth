/*
 * midi.h
 *
 *  Created on: 13 juli 2020
 *      Author: Mikael Hessel
 */

#ifndef INC_MODULES_MIDI_H_
#define INC_MODULES_MIDI_H_

// USB and midi drivers
#include "usb_host.h"  // for usb conncection
#include "external/usbh_MIDI.h"
#include "usbh_def.h"

// for note data, replace with linked list of notes later
#include "modules/sound.h"

// for parameter change.. maybe these global parameters could be collected in a separate config file...
#include "modules/delay.h"

#include <stdlib.h>
#include <stdint.h>

#define MIDI_BUF_SIZE	64

// for midi buffer
uint8_t midiRxBuf[MIDI_BUF_SIZE];


void parseMidi();


void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost);

#endif /* INC_MODULES_MIDI_H_ */
