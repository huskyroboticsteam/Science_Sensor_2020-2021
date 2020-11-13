#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"
#include "timers.h"

int32_t encoder0_ticks;
int32_t encoder1_ticks;

/*Returns the total number of encoder ticks on the specified encoder since the last reset*/
int32_t get_encoder_ticks(uint8_t which){
	if(which){
		return encoder1_ticks;
	}
	return encoder0_ticks;
}

/*Sets the specified encoder counter to the specified value*/
void set_encoder_ticks(uint8_t which, int32_t ticks){
	if(which){
		encoder1_ticks = ticks;
	} else {
		encoder0_ticks = ticks;
	}
}

/*Initializes the encoders*/
void init_encoders(){
	EIMSK &= 0x33; //Disable pin change interrupts
	DDRE &= 0x3F; //Set PE6, PE7 as inputs
	DDRD &= 0xF3; //Set PD2, PD3 as inputs
	PORTE |= 0xC0; //Enable pullups on PE6, PE7
	PORTD |= 0x0C; //Enable pullups on PD2, PD3
	encoder0_ticks = 0; //Reset count
	encoder1_ticks = 0;
	EICRB |= 0x50; //Enable pin change interrupt on PE6, PE7
	EICRA |= 0x50; //Enable pin change interrupt on PD2, PD3
	EIMSK |= 0xCC; //Re-enable pin change mask for 2, 3, 6, 7
}

/*Resets the specified encoder count. Passing which = 2 will reset both*/
void reset_encoder(uint8_t which){
	EIMSK &= 0x33; //Disable pin change interrupts
	switch(which){
		case 0:
			encoder0_ticks = 0;
			break;
		case 2:
			encoder0_ticks = 0;
		case 1:
			encoder1_ticks = 0;
			break;
	}
	EIMSK |= 0xCC; //Enable pin change interrupt on PE6, PE7
}

ISR(INT6_vect){ //PE6, A
	uint8_t state = PINE;
	if(state & (1<<PE6)){ //A rising
		if(state & (1<<PE7)){ //B high
			encoder0_ticks++;
		} else { //B low
			encoder0_ticks--;
		}
	} else { //A falling
		if(state & (1<<PE7)){ //B high
			encoder0_ticks--;
		} else { //B low
			encoder0_ticks++;
		}
	}
}

ISR(INT7_vect){ //PE7, B
	uint8_t state = PINE;
	if(state & (1<<PE7)){ //B rising
		if(state & (1<<PE6)){ //A high
			encoder0_ticks--;
		} else { //A low
			encoder0_ticks++;
		}
	} else { //B falling
		if(state & (1<<PE6)){ //A high
			encoder0_ticks++;
		} else { //A low
			encoder0_ticks--;
		}
	}
}

ISR(INT2_vect){ //PD2, A
	uint8_t state = PIND;
	if(state & (1<<PD2)){ //A rising
		if(state & (1<<PD3)){ //B high
			encoder1_ticks++;
			} else { //B low
			encoder1_ticks--;
		}
		} else { //A falling
		if(state & (1<<PD3)){ //B high
			encoder1_ticks--;
			} else { //B low
			encoder1_ticks++;
		}
	}
}

ISR(INT3_vect){ //PD3, B
	uint8_t state = PIND;
	if(state & (1<<PD3)){ //B rising
		if(state & (1<<PD2)){ //A high
			encoder1_ticks--;
			} else { //A low
			encoder1_ticks++;
		}
		} else { //B falling
		if(state & (1<<PD2)){ //A high
			encoder1_ticks++;
			} else { //A low
			encoder1_ticks--;
		}
	}
}