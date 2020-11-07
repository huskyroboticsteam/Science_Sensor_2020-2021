#ifndef I2C_H
#define I2C_H

#include <inttypes.h>

void I2C_init(uint16_t bitrate);

void I2C_start();

void I2C_stop();

void I2C_write(uint8_t byte);

uint8_t I2C_read(uint8_t ack);

#endif