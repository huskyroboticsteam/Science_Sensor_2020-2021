#ifndef DAC_H
#define DAC_H

#include <avr/pgmspace.h>
#include <inttypes.h>

void dac_write(uint8_t val);
void play_audio(const uint8_t *PROGMEM samples, uint16_t len);
void play_tone(uint16_t freq, uint16_t duration);
void roll();

#endif