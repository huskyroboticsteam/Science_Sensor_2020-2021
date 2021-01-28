#include <avr/io.h>
#include <i2c.h>
#include <usart.h>
#include <avr/wdt.h>

#define VEML6070_ADDR_ARA 0x18
#define VEML6070_ADDR_CMD 0x70
#define VEML6070_ADDR_DATA_LSB 0x71
#define VEML6070_ADDR_DATA_MSB 0x73

uint8_t VEML6070_read_byte(uint8_t addr){
	I2C_start();
	I2C_write(addr);
	uint8_t val = I2C_read(1);
	I2C_stop();
	return val;
}

void VEML6070_write_byte(uint8_t addr, uint8_t val){
	I2C_start();
	I2C_write(addr);
	I2C_write(val);
	I2C_stop();
}

void VEML6070_init(){
	wdt_reset();
	VEML6070_read_byte(VEML6070_ADDR_ARA);
	VEML6070_write_byte(VEML6070_ADDR_CMD, 0x04); //Integration time = 1
}

uint16_t read_uv_sensor(){
	uint8_t lsb = VEML6070_read_byte(VEML6070_ADDR_DATA_LSB);
	uint8_t msb = VEML6070_read_byte(VEML6070_ADDR_DATA_MSB);
	return (msb << 8) | lsb;
}