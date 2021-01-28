#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

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
#include "uv.h"
#include "meow.h"

void handle_CAN_packet(CANPacket *packet);

int main(){
	DDRB = 1<<6; // Enable LED pins
	PORTB = 1<<6;
	DDRG |= 1<<4;
	PORTB = 0;
	PORTA = 0xF0; //Enable DIP switch pullups
	uint8_t H[3] = {64, 64, 64};
	uint8_t L[3] = {0, 0, 0};
	set_RGB(L, L, L);
	_delay_ms(333);
	set_RGB_LED(0, 0, 128, 0);
	setup_timers();
	_delay_ms(500);
	init_ADC();
	set_RGB_LED(1, 0, 128, 0);
	delay_mS(500);
	I2C_init(333);
	usart_init(19200);
	set_RGB_LED(2, 0, 128, 0);
	delay_mS(500);
	cli();
	set_RGB(H, L, L);
	DDRD = 1<<4; //Enable audio amp
	PORTD = 1<<4;
	_delay_ms(100);
	play_audio(meow, sizeof(meow));
	set_RGB(L, L, L);
	sei();
	uint8_t count = eeprom_read_byte(0);
	eeprom_write_byte(0, count + 1);
	if(count % 32 == 0){
		roll();	
	}
	PORTD = 0; //Disable audio amp
	wdt_enable(WDTO_2S); //Watchdog interval 2 secs
	InitCAN(DEVICE_GROUP_SCIENCE, get_dip_switch());
	init_servos();
	init_motor();
	VEML6070_init(); /*UV sensor*/
	CANPacket packet;
	while(1){
		if(PollAndReceiveCANPacket(&packet) == 0){
			set_LED(LED_CAN, 3);
			update_LEDS(get_mS()/40);
			handle_CAN_packet(&packet);
			set_LED(LED_CAN, 0);
		}
		motor_control_tick();
		_delay_ms(25);
		wdt_reset();
	}
}