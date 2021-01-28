#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include "timers.h"
#include "usart.h"
#include "i2c.h"

/*Initialize I2C
  The bitrate of the I2C bus in KHz
*/
void I2C_init(uint16_t bitrate){
	//PORTD |= 3; //Enable pullup
	TWSR = 0; //No Prescaling
	TWBR = ((16000L / bitrate) - 16) / 2;
	TWCR = 1 << TWEN; //Enable I2C
}

/*Start an I2C transaction*/
void I2C_start(){
	TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));
	while(!(TWCR & (1<<TWINT)));
}

/*Stop an I2C transaction*/
void I2C_stop(){
    TWCR = ((1<< TWINT) | (1<<TWEN) | (1<<TWSTO));
	//_delay_us(10);
}

/*Write a byte to a I2C device*/
void I2C_write(uint8_t byte){
	TWDR = byte;
	TWCR = ((1<< TWINT) | (1<<TWEN));
	while(!(TWCR & (1 <<TWINT))); //Wait for write to complete
}

/*Read a byte from an I2C device
	Setting ack to one enables sending an acknowledge pulse*/
uint8_t I2C_read(uint8_t ack){
	uint32_t ms = get_mS();
	TWCR = ((1<< TWINT) | (1<<TWEN) | ((!!ack)<<TWEA));
	while(!(TWCR & (1 <<TWINT))){ //Wait for something to read
		if(get_mS() - ms > 250){
			set_LED(LED_ERR, 3);
			update_LEDS(get_mS()/40);
			tprintf("I2C read timeout\n");
			set_LED(LED_ERR, 0);
			break;
		}
	}
	return TWDR;
}