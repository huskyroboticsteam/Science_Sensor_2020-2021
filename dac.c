/*
 * dac.c
 *
 * Created: 11/7/2020 12:47:48 PM
 *  Author: John
 */ 
#include "i2c.h"
#include "dac.h"
#include "timers.h"
#include "usart.h"
#include "led.h"
#include <util/delay.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#define DAC_ADDR 0x60

uint8_t sine_table[] = {0x80, 0xB0, 0xDA, 0xF5, 0xFF, 0xF5, 0xDA, 0xB0, 0x80, 0x4F, 0x25, 0x0A, 0x00, 0x0A, 0x25, 0x4F};
	
uint16_t freqs[] = {233, 262, 277, 277, 311, 262, 233, 208, 233, 233, 262, 277, 233, 208, 415, 415, 311, 233, 233, 262, 277, 233, 277, 311, 262, 233, 262, 233, 208, 233, 233, 262, 277, 233, 208, 311, 311, 311, 349, 311, 277, 311, 349, 277, 311, 311, 311, 349, 311, 208, 208, 233, 262, 277, 233, 311, 349, 311};
	
uint16_t times[] = {191, 192, 192, 192, 192, 192, 192, 1536, 192, 192, 192, 192, 383, 192, 384, 192, 1152, 192, 192, 192, 192, 192, 192, 384, 192, 192, 192, 192, 764, 192, 192, 192, 192, 192, 384, 192, 192, 192, 191, 768, 960, 192, 192, 192, 192, 192, 192, 192, 192, 192, 1151, 192, 192, 192, 384, 192, 192, 576};

void roll(){
	uint8_t R[3] = {0, 255, 255};
	uint8_t G[3] = {64, 0, 255};
	uint8_t B[3] = {0, 255, 255};
	uint8_t *ptrs[3] = {R, G, B};
	for(int i = 0;i < sizeof(freqs)/2;i++){
		play_tone(freqs[i], times[i]/3);
		_delay_ms(50);
		ptrs[rand() % 3][rand() % 3] = (rand() % 4) * 16;
		set_LED(rand() % 3, rand() & 1 ? 3 : 0);
		update_LEDS(0);
		set_RGB(R, G, B);
	}
	_delay_ms(50);
	R[2] = R[1] = R[0] = 0;
	set_RGB(R, R, R);
	set_LED(0, 0);
	set_LED(1, 0);
	set_LED(2, 0);
	_delay_ms(50);
}

void play_tone(uint16_t freq, uint16_t duration){
	uint32_t start = get_mS();
	uint8_t t = 0;
	I2C_init(freq/2 - freq/100);
	while(get_mS() - start < duration){
		dac_write(sine_table[t % sizeof(sine_table)]);
		t++;
	}
	dac_write(0);
	I2C_init(333);
}

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