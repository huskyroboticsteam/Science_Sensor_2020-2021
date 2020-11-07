#include <avr/io.h>
#include "config.h"
#include "usart.h"
#include "util.h"

int32_t min(int32_t a, int32_t b){
	return (a < b)? a:b;
}

int32_t max(int32_t a, int32_t b){
	return (a > b)? a:b;
}

/*Integer implementation of abs()*/
uint16_t int_abs(int16_t x){
	return x<0? -x: x;
}


/*Reads a string from uart
Parameters:
char *buffer: the buffer to put the string
char until: The deliminating character
*/
void read_string(char *buffer, char until){
	char c = -1;
	int i = 0;
	while(c != until){
		c = usart_read_char();
		tprintf("%c", c);
		buffer[i++] = c;
	}
	buffer[i-1] = 0;
}