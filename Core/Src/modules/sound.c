/*
 * sound.c
 *
 *	Sound generation functions
 *
 *	Also functions for controlling CS43L22 DAC chip
 *
 *  Created on: 15 juni 2020
 *      Author: Mikael Hessel
 */

#include "modules/sound.h"

#include "i2s.h"

#include <stdint.h>
#include <math.h>  // needed for pow function

// plays a note at frequencey frq for dur time (milliseconds)
uint32_t playSound(uint8_t note, uint16_t time, float f, uint8_t wave)
{
	uint32_t r;
	// float frq = noteToFreq(note);
	r = osc(f, wave, time);
	return r;
}



uint16_t osc(float f, enum osctype ot, uint16_t time){
	float dt = f/SRATE;
	uint16_t retval = 0;
	uint16_t idt;

	switch(ot){
	case SINUS:
		// scale result to 12 bit
		retval = ((sinf(time * TWOPI * dt ) + 1) * 2047);
		break;
	case SAWTOOTH:
		idt = SRATE/f;
		retval = (time % idt) * (4095 / idt);
		break;
	case SQUARE:
		idt = SRATE/f;
		retval = ( (time % idt < idt/2) ? 0 : 4095);
		break;
	default:
		break;
	}
	return retval;
}

// convert a note (enum) to a frequency (use midi note data as starting pont?)
// Midi has 128 notes from 0 (C -2) to 127 (F# 7)
// A 4 is 81

// Faster if this is made as table lookup!

float noteToFreq(uint8_t note)
{
	// Twelfth root of 2 as ratio
	// A-4 as starting frequency

	return A_FOURTH * pow(FRQ_RATIO, note - 81) ; // 81 is midi note value of A 4
}


// set up CS43L22 for analog passthrough
// with som help from: https://github.com/MYaqoobEmbedded
void initCS43(I2C_HandleTypeDef* c43i2c)
{
	// temp variable for sending and receving data
	uint8_t tempdata = 0x00;

	// Enable I2S3
	  __HAL_UNLOCK(&hi2s3);
	  __HAL_I2S_ENABLE(&hi2s3);

	  // set up CS43L22 for output from internal DAC (PA4)
	  // CS43ADDR  write, 0x95 read

	  // Bring reset high to enable chip
	  HAL_GPIO_WritePin(Audio_RST_GPIO_Port, Audio_RST_Pin, GPIO_PIN_SET);

	  HAL_Delay(5); // Short delay after pulling reset high

	  // ( MCLK is enabled by starting I2S transfer in main)

	  // Ensure power down mode (set 0x02 to 0x01)
	  tempdata = 0x01;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x02, 1, &tempdata, 1, 50);

	  // Setup as Interface 1 Control (0x06)
	  // Slave mode, SCLK non inveted, DSP disabled, DAC I2S up to 24 bit, AWL 16-bit
	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x06, 1, &tempdata, 1, 50);
	  tempdata &= 0x20;
	  tempdata |= 0x07;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x06, 1, &tempdata, 1, 50);



	  // set registers 0x08 and 0x09 (analog passthrough A and B) to AN1x
	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x06, 1, &tempdata, 1, 50);
	  tempdata &= 0xF0;
	  tempdata |= 0x01;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x08, 1, &tempdata, 1, 50);

	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x06, 1, &tempdata, 1, 50);
	  tempdata &= 0xF0;
	  tempdata |= 0x01;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x09, 1, &tempdata, 1, 50);

	  // 0x0E bits 7 and 6 are analog passthrough enable
	  // bits 5 and 4 are PASSMUTE and should be 0
	  // set passthrought bits (first read register, then OR the bits and write back)
	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x0E, 1, &tempdata, 1, 50);
	  tempdata |= 0xC0;
	  tempdata &= 0xCF;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x0E, 1, &tempdata, 1, 50);

	  // 0x14 and 0x15 are passthrough volume, 0x00 is 0dB
	  tempdata = 0x00;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x14, 1, &tempdata, 1, 50);
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x15, 1, &tempdata, 1, 50);

}

// start CS43L22 with init sequence
void startCS43(I2C_HandleTypeDef* c43i2c)
{
	// temp variable for sending and receving data
	uint8_t tempdata = 0x00;

	  // do init sequence
	  /*
	1. Write 0x99 to register 0x00.
	2. Write 0x80 to register 0x47.
	3. Write ‘1’b to bit 7 in register 0x32.
	4. Write ‘0’b to bit 7 in register 0x32.
	5. Write 0x00 to register 0x00.
	   */
	  tempdata = 0x99;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x00, 1, &tempdata, 1, 50);
	  tempdata = 0x80;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x47, 1, &tempdata, 1, 50);

	  HAL_I2C_Mem_Read(c43i2c, CS43ADDR, 0x32, 1, &tempdata, 1, 50);
	  tempdata |= 0x80;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x32, 1, &tempdata, 1, 50);
	  // HAL_I2C_Mem_Read(&hi2c1, CS43ADDR, 0x32, 1, &tempdata, 1, 50);
	  tempdata &= 0x7F;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x32, 1, &tempdata, 1, 50);

	  tempdata = 0x00;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x00, 1, &tempdata, 1, 50);

	 // while(HAL_I2C_IsDeviceReady(&hi2c1, CS43ADDR, 10, 1000) != HAL_OK) {}

	  // Set power ctl 1 (0x02) to 0x9E to do start
	  tempdata = 0x9e;
	  HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x02, 1, &tempdata, 1, 50);
}

void stopCS43(I2C_HandleTypeDef* c43i2c)
{
	// temp variable for sending data
	uint8_t tempdata = 0x01;

	// write to power ctl 1
	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, 0x02, 1, &tempdata, 1, 50);
}


// Volume setting from  0 to 255
// 0x00 - 0x18 unusable. normal volume at 0xe7
void setVolCS43(I2C_HandleTypeDef* c43i2c, uint8_t vol)
{
	// max digital volume is +12 db at 0x18;
	uint8_t dvol = ((vol> 0x6e)? (vol - 0xe7):(vol+0x19));

	// max passthrough volume is +12db
	uint8_t avol = ((vol > 0x7f)? (vol - 0x80):(vol + 0x80));


	// Master volume at 0x20, 0x21

	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, MASTER_VOLA, 1, &dvol, 1, 50);
	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, MASTER_VOLB, 1, &dvol, 1, 50);

	// 0x14 and 0x15 are passthrough volume, 0x00 is 0dB
	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, PASSTHU_VOLA, 1, &avol, 1, 50);
	HAL_I2C_Mem_Write(c43i2c, CS43ADDR, PASSTHU_VOLB, 1, &avol, 1, 50);

}
