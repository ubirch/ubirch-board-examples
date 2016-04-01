/**
 * ubirch#1 i2c driver code (blocking).
 *
 * Driver for I2C chipset on the ubirch#1.
 * This code uses the synchronous functions for executing i2c transfers.
 *
 * The I2C pins are A4/A5 (PTB10/PTB11 Alt4) and the bus is referenced
 * in the Kinetis SDK as I2C2.
 *
 * @author Matthias L. Jugel
 * @date 2016-04-01
 *
 * Copyright 2016 ubirch GmbH (https://ubirch.com)
 *
 * == LICENSE ==
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _UBIRCH_I2C_CORE_H
#define _UBIRCH_I2C_CORE_H

#include <drivers/fsl_common.h>

/**
 * Available I2C speeds.
 */
typedef enum i2c_speed {
    I2C_STANDARD   =  100000U, // 100kHz
    I2C_FULL_SPEED =  400000U, // 400kHz
    I2C_FAST_MODE  = 1000000U, //   1MHz
    I2C_HIGH_SPEED = 3400000U  // 3.4MHz
} i2c_speed_t;

/**
 * Initialize the I2C bus with a specific speed.
 * @param speed bus speed
 */
void i2c_init(i2c_speed_t speed);

/**
 * Deinitialize I2C bus
 */
void i2c_deinit();

/**
 * Ping an I2C device on the bus, will return kStatus_Success if available.
 * @param address the 7 bit device address
 */
status_t i2c_ping(uint8_t address);

/**
 * Write data to a specific register on the bus.
 * @param address the 7 bit device address
 * @param reg the register to write to
 * @param data a data array to write
 * @param size the amount of bytes to write
 */
status_t i2c_write_reg(uint8_t address, uint8_t reg, uint8_t *data, size_t size);

/**
 * Read a single byte from a register.
 * @param address the 7 bit device address
 * @param reg the register to read from
 */
uint8_t i2c_read_reg(uint8_t address, uint8_t reg);

/**
 * Read a two byte integer from a register
 * @param address the 7 bit device address
 * @param reg the register to read from
 */
uint16_t i2c_read_reg16(uint8_t address, uint8_t reg);

#ifndef NDEBUG
/**
 * Write a human readable I2C response code translation
 * to the debug console including explanation.
 * @param s a string with details on the current operation
 * @param status the status_t reference to check
 */
void i2c_error(char *s, status_t status);
#endif

#endif // _UBIRCH_I2C_CORE_H