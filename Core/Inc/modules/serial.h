/*
 * serial.h
 *
 *  Created on: Jun 10, 2020
 *      Author: Mikael Hessel
 *
 *  Functions for serial port communication (debug and user interface testing)
 *
 */

#ifndef INC_MODULES_SERIAL_H_
#define INC_MODULES_SERIAL_H_

// Setting serial port

#define SERIALPORT &huart2	// alias for serial port
#define TXTIMEOUT 1000 		// 1 second timeout on send
#define RXTIMEOUT 1000		// 1 second timeout on receive

void serialprint(char* str);

/*
 * queue print
 *
 */

#endif /* INC_MODULES_SERIAL_H_ */
