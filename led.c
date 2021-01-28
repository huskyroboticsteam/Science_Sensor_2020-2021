#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"

uint8_t LED_states;
uint8_t RS[3] = {0, 0, 0};
uint8_t GS[3] = {0, 0, 0};
uint8_t BS[3] = {0, 0, 0};

/*Sets the mode of the debug LEDs
Parameters:
uint8_t L: the number of the LED to set (0-2)
	0 -> CAN
	1 -> DBG
	2 -> ERR
uint8_t state: the mode for the LED:
	0 -> Off
	1 -> Slow flashing
	2 -> Quick flashing
	3 -> Steady on
*/
void set_LED(uint8_t L, uint8_t state){
	if(L > 2 || state > 3) return;
	DDRB |= (1<<7 | 1<<6);
	DDRG |= 1<<3;
	L <<= 1;
	LED_states = (LED_states & ~(3 << L)) | (state << L);
}

/*Call this from a timer interrupt to update the debug LEDs
Parameters:
uint16_t count: The timer count
*/
void inline update_LEDS(uint16_t count){
	int i;
	uint8_t v;
	if(!LED_states){
		PORTG &= ~(1 << 3);
		PORTB &= ~(1 << 7 | 1 << 6);
		return;
	}
	for(i = 0;i < 3;i++){
		v = (LED_states >> (2*i)) & 3;
		switch(v){
			case 0:
				if(i == 0){
					PORTB &= ~(1<<6);
				} else if(i == 1){
					PORTB &= ~(1<<7);
				} else { //i == 2
					PORTG &= ~(1<<3);
				}
				break;
			case 1:
				if(count % 4 != 0){
					return;
				}
				if(i == 0){
					PORTB ^= 1<<6;
					} else if(i == 1){
					PORTB ^= 1<<7;
					} else { //i == 2
					PORTG ^= 1<<3;
				}
				break;
			case 2:
				if(count % 2 != 0){
					return;
				}
				if(i == 0){
					PORTB ^= 1<<6;
				} else if(i == 1){
					PORTB ^= 1<<7;
				} else { //i == 2
					PORTG ^= 1<<3;
				}
				break;
			case 3:
				if(i == 0){
					PORTB |= 1<<6;
				} else if(i == 1){
					PORTB |= 1<<7;
				} else { //i == 2
					PORTG |= 1<<3;
				}
				break;
				
		}
	}
}

/*Sets the color of the RGB LEDs
 Where <led> is the led number from 0-2
 R[led] = the red value (0-255) for LED <led>
 G[led] = the green value (0-255) for LED <led>
 B[led] = the blue value (0-255) for LED <led>
*/
void set_RGB(uint8_t *R, uint8_t *G, uint8_t *B){
	cli();
	for(int led = 0;led < 3;led++){
		uint8_t r = R[led];
		uint8_t g = G[led];
		uint8_t b = B[led];
		uint8_t count = 8;
		while(count--){
			uint8_t res = g & 128;
			PORTG |= 1<<4;
			g <<= 1;
			if(res){
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			}
			PORTG &= ~(1<<4);
			if(!res){
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			}
			asm("nop");
		}
		count = 8;
		while(count--){
			uint8_t res = r & 128;
			PORTG |= 1<<4;
			r <<= 1;
			if(res){
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			}
			PORTG &= ~(1<<4);
			if(!res){
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			}
			asm("nop");
		}
		count = 8;
		while(count--){
			uint8_t res = b & 128;
			PORTG |= 1<<4;
			b <<= 1;
			if(res){
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			}
			PORTG &= ~(1<<4);
			if(!res){
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
				asm("nop");
			}
			asm("nop");
		}
	}
	sei();
}

/*Sets the color of RGB LED <led> to the specified R G B values*/
void set_RGB_LED(uint8_t led, uint8_t R, uint8_t G, uint8_t B){
	RS[led] = R;
	GS[led] = G;
	BS[led] = B;
	set_RGB(RS, GS, BS);
}