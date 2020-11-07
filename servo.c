#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <timers.h>
#include <config.h>
#include <servo.h>

#include "i2c.h"

#define PCA9685_ADDR    0x40

#define PCA9685_MODE1_REG       0x00
#define PCA9685_MODE2_REG       0x01
#define PCA9685_PRESCALE        0xFE

#define PCA9685_PIN    0x06

#define PCA9685_SLEEP_BIT       0x10
#define PCA9685_RESTART_BIT     0x80

void PCA9685_write(uint8_t reg, uint8_t val){
	I2C_start();
	I2C_write(PCA9685_ADDR << 1);
	I2C_write(reg);
	I2C_write(val);
	I2C_stop();
}

void PCA9685_init(){
	PCA9685_write(PCA9685_MODE1_REG, PCA9685_RESTART_BIT);
	_delay_ms(10);
	PCA9685_write(PCA9685_MODE1_REG, PCA9685_SLEEP_BIT);
	_delay_ms(2);
	PCA9685_write(PCA9685_MODE1_REG, 0x00);
	_delay_ms(2);
}

void PCA9685_set_freq(uint16_t freq){
	uint8_t prescale = ((25000000L / 4096) / freq) - 1;
	I2C_start();
	I2C_write((PCA9685_ADDR << 1) | 1);
	I2C_write(PCA9685_MODE1_REG);
	I2C_start();
	uint8_t old_mode = I2C_read(0);
	I2C_stop();
	
	PCA9685_write(PCA9685_MODE1_REG, old_mode & 0x7F | PCA9685_SLEEP_BIT);
	_delay_ms(2);
	PCA9685_write(PCA9685_PRESCALE, prescale);
	PCA9685_write(PCA9685_MODE1_REG, old_mode);
	_delay_ms(2);
	PCA9685_write(PCA9685_MODE1_REG, old_mode | PCA9685_RESTART_BIT);
	_delay_ms(2);
}

void PCA9685_set_pwm(uint8_t pin, uint16_t pwm){
	PCA9685_write(PCA9685_PIN + 4 * pin , (4095 - pwm) & 0xFF);
	PCA9685_write(PCA9685_PIN + 4 * pin + 1, (4095 - pwm) >> 8);
	PCA9685_write(PCA9685_PIN + 4 * pin + 2 , pwm & 0xFF);
	PCA9685_write(PCA9685_PIN + 4 * pin + 3, pwm >> 8);
}

void init_servos(){
	PCA9685_init();
	PCA9685_set_freq(50);	
}

void set_servo_position(int servo, int degrees){
	if(degrees > 179) degrees = 179;
	if(degrees < 1) degrees = 1;
	if(servo < 0 || servo > 2){
		return;
	}
	PCA9685_set_pwm(servo, 102 + (degrees * 99) / 172);
}