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

int16_t motor_powers[3];
int32_t last_update[3];

/*Initializes the motor controller and sets some reasonable defaults*/
void init_motor(){
	DDRE |= (1 << 3) | (1 << 4) | (1 << 5); //set motor pwm pins to outputs
	DDRB |= (1 << 3) | (1 << 4) | (1 << 5); //set motor direction pins to outputs
	for(int m = 0;m < 3;m++){ //Set the inital motor powers to 0
		motor_powers[m] = 0;
		write_PWM(m + 3, 0);
	}
	init_encoders();
}

void set_motor_direction(uint8_t motor, uint8_t dir){
	dir = !!dir;
	if(motor > 2){
		return;
	}
	motor += 3;
	dir <<= motor;
	PORTB &= ~dir;
	PORTB |= dir;
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
	for(uint8_t motor = 0;motor < 3;motor++){
		if(get_mS() - last_update[motor] > MOTOR_SET_TIMEOUT){
			/*If more than set time has elapsed without an update to this motor, turn it off*/
			motor_powers[motor] = 0;
		}
		int16_t power = motor_powers[motor];
		if(power < 0){
			set_motor_direction(motor, 1);
			power = -power;
		} else {
			set_motor_direction(motor, 0);
		}
		write_PWM(motor + 3, power);
	}
}