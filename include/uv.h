#ifndef UV_H
#define UV_H

uint8_t VEML6070_read_byte(uint8_t addr);
void VEML6070_write_byte(uint8_t addr, uint8_t val);
void VEML6070_init();
uint16_t read_uv_sensor();

#endif