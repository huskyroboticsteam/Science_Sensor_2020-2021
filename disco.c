#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>

#include "CANLibrary.h"

#include "pwm.h"
#include "encoder.h"
#include "motor.h"
#include "timers.h"
#include "adc.h"
#include "can.h"
#include "messaging.h"
#include "can.h"
#include "util.h"
#include "servo.h"
#include "usart.h"
#include "led.h"




int main(){
	DDRG = 1<<4 | 1<<3;
	DDRB = 1<<6 | 1<<7;
	DDRB |= 1 | 2 | 4;
	uint8_t R[3] = {0, 255, 255};
	uint8_t G[3] = {64, 0, 255};
	uint8_t B[3] = {0, 255, 255};
	uint8_t *ptrs[3] = {R, G, B};
	while(1){
		ptrs[rand() % 3][rand() % 3] = rand() % 128;
		set_LED(rand() % 3, rand() & 1 ? 3 : 0);
		PORTB = rand() & 7;
		update_LEDS(0);
		set_RGB(R, G, B);
		_delay_ms(70);
	}
}