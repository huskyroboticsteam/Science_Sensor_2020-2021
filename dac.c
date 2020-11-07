/*
 * dac.c
 *
 * Created: 11/7/2020 12:47:48 PM
 *  Author: John
 */ 
#include "i2c.h"
#include <inttypes.h>
#include <avr/pgmspace.h>

#define DAC_ADDR 0x60

uint8_t sine_table[] = {0x80, 0xB0, 0xDA, 0xF5, 0xFF, 0xF5, 0xDA, 0xB0, 0x80, 0x4F, 0x25, 0x0A, 0x00, 0x0A, 0x25, 0x4F};

void dac_write(uint8_t val){
	I2C_start();
	I2C_write(DAC_ADDR << 1); //DAC address in write mode
	I2C_write(0); //power-down bits = 00 -> normal op, don't care data bits = 0000
	I2C_write(val); //next 8 bits is the DAC register value
	I2C_stop();
}

void play_audio(const uint8_t *PROGMEM samples, uint16_t len){
	for(uint16_t i = 0;i < len;i++){
		dac_write(pgm_read_byte(samples + i));
	}
}