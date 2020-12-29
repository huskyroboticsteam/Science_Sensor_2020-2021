#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>

#include <avr/pgmspace.h>

#include "CANLibrary.h"

#include "pwm.h"
#include "encoder.h"
#include "motor.h"
#include "timers.h"
#include "adc.h"
#include "messaging.h"
#include "util.h"
#include "servo.h"
#include "usart.h"
#include "led.h"
#include "i2c.h"
#include "dac.h"
#include "meow.h"

void handle_CAN_packet(CANPacket *packet);

int main(){
	DDRD = 1<<4;
	PORTD = 1<<4;
	DDRB = 1<<6;
	PORTB = 1<<6;
	PORTA = 0xF0; //Enable DIP switch pullups
	_delay_ms(250);
	DDRG |= 1<<4;
	PORTB = 0;
	uint8_t H[3] = {64, 64, 64};
	uint8_t L[3] = {0, 0, 0};
	setup_timers();
	I2C_init(333);
	cli();
	set_RGB(H, L, L);
	_delay_ms(100);
	//play_audio(meow, sizeof(meow));
	set_RGB(L, L, L);
	//dac_write(0);
	PORTD = 0;
	sei();
	//wdt_enable(WDTO_2S);
	InitCAN(DEVICE_GROUP_SCIENCE, get_dip_switch());
	init_servos();
	init_motor();
	/*	DDRE |= 8 | 16 | 32;
		DDRB |= 8 | 16 | 32;
		//PORTE = 8;
		write_PWM(3, 10);
		while(1);*/
	CANPacket packet;
	while(1){
		if(PollAndReceiveCANPacket(&packet) == 0){
			//set_LED(LED_CAN, 3);
		//	update_LEDS(get_mS()/40);
			handle_CAN_packet(&packet);
			//set_LED(LED_CAN, 0);
			//update_LEDS(get_mS()/40);
		}
		motor_control_tick();
		wdt_reset();
	}
}