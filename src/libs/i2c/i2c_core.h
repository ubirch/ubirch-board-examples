#ifndef _UBIRCH_I2C_CORE_H
#define _UBIRCH_I2C_CORE_H

#include <drivers/fsl_common.h>

typedef enum i2c_speed {
    I2C_STANDARD   =  100000U, // 100kHz
    I2C_FULL_SPEED =  400000U, // 400kHz
    I2C_FAST_MODE  = 1000000U, //   1MHz
    I2C_HIGH_SPEED = 3400000U  // 3.4MHz
} i2c_speed_t;

/**
 * Initialize I2C2 with a specific speed.
 * @param speed
 */
void i2c_init(i2c_speed_t speed);

void i2c_deinit();

status_t i2c_ping(uint8_t address);

status_t i2c_write_reg(uint8_t address, uint8_t reg, uint8_t *data, size_t size);

uint8_t i2c_read_reg(uint8_t address, uint8_t reg);

uint16_t i2c_read_reg16(uint8_t address, uint8_t reg);

void i2c_error(char *s, status_t status);

#endif // _UBIRCH_I2C_CORE_H
