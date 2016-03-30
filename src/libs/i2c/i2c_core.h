#ifndef _UBIRCH_I2C_CORE_H
#define _UBIRCH_I2C_CORE_H

#include <drivers/fsl_common.h>

void i2c_init(uint32_t speed);

status_t i2c_ping(uint8_t address);

status_t i2c_write_reg(uint8_t address, uint8_t reg, uint8_t *data, size_t size);

uint8_t i2c_read_reg(uint8_t address, uint8_t reg);

uint16_t i2c_read_reg16(uint8_t address, uint8_t reg);

void i2c_error(char *s, status_t status);

#endif // _UBIRCH_I2C_CORE_H
