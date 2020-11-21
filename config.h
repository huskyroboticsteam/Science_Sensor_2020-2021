#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <avr/io.h>

#define F_CPU 16000000L

#define MOTOR_SET_TIMEOUT 2000

#define USART_TX_BUF_SZ 16 //UART TX buffer size
#define USART_RX_BUF_SZ 16 //UART RX buffer size

/*Returns the value of the binary representation of the dipswitch*/
uint8_t inline get_dip_switch(){
	return ((~PINA) >> 4) & 0xF;
}

#endif