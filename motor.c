#include <avr/io.h>
#include "config.h"
#include "timers.h"
#include "pwm.h"
#include "adc.h"
#include "encoder.h"
#include "motor.h"
#include "messaging.h"
#include "usart.h"
#include "util.h"
#include "led.h"

int16_t motor_powers[3];
int32_t last_update[3];
uint8_t motor_leds = 1;

/*Initializes the motor controller and sets some reasonable defaults*/
void init_motor(){
	DDRE |= (1 << 3) | (1 << 4) | (1 << 5); //set motor pwm pins to outputs
	DDRB |= (1 << 3) | (1 << 4) | (1 << 5); //set motor direction pins to outputs
	for(int m = 0;m < 3;m++){ //Set the inital motor powers to 0
		motor_powers[m] = 0;
		write_PWM(m + 3, 0);
	}
	PORTC = 1<<5 | 1<<6; //enable limit switch pullups
	init_encoders();
}

void set_motor_direction(uint8_t motor, uint8_t dir){
	if(motor > 2){
		return;
	}
	motor += 3;
	uint8_t bit = 1 << motor;
	if(!dir){
		PORTB &= ~bit;
	} else {
		PORTB |= bit;
	}
}

/*Enables or disables whether to use the RGB LEDS to indicate motor states*/
void set_motor_leds(uint8_t en){
	motor_leds = !!en;
}

/*Sets the motor power more safely
  Parameters:
  uint8_t motor: selects which motor to set the power of
  int16_t power: the motor power to set -1023 to +1023
  Negative values reverse the motor*/
void set_motor_power(uint8_t motor, int16_t power){
	if(motor < 0 || motor > 2){
		return;
	}
	if(power < -1023){
		power = -1023;
	}
	if(power > 1023){
		power = 1023;
	}
	motor_powers[motor] = power;
	last_update[motor] = get_mS();
}

/*Update the motors. Call in a loop*/
void motor_control_tick(){
	volatile uint8_t ls1 = PINC & (1<<6);
	volatile uint8_t ls2 = PINC & (1<<5);
	//volatile uint8_t p = PINC;
	for(uint8_t motor = 0;motor < 3;motor++){
		if(get_mS() - last_update[motor] > MOTOR_SET_TIMEOUT){
			/*If more than set time has elapsed without an update to this motor, turn it off*/
			motor_powers[motor] = 0;
		}
		int16_t power = motor_powers[motor];
		if(power < 0){
			/*stop the motors if the limit switches are triggered*/
			if(motor == 0 && ls1 == 0){
				reset_encoder(0);
				power = 0;
				if(motor_leds){
					set_LED(LED_DBG, 3);
					update_LEDS(get_mS()/40);
					set_LED(LED_DBG, 0);
				}
			}
			if(motor == 1 && ls2 == 0){
				reset_encoder(1);
				power = 0;
				if(motor_leds){
					set_LED(LED_DBG, 3);
					update_LEDS(get_mS()/40);
					set_LED(LED_DBG, 0);
				}
			}
			set_motor_direction(motor, 1);
			power = -power;
		} else {
			set_motor_direction(motor, 0);
		}
		if(motor_leds){
			set_RGB_LED(motor, 0, power / 16, 0);
		}
		write_PWM(motor + 3, power);
	}
}