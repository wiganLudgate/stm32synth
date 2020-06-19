/*
 * test_serial.c
 *
 *  Created on: 11 juni 2020
 *      Author: Mikael Hessel
 */

#include "modules/test/test_serial.h"
#include "modules/serial.h"

void test_serial_all()
{
	test_serialprint();
}

void test_serialprint()
{
	serialprint("Teststring\n");
}
