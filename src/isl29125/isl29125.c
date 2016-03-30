/**
 * RGB sensor library (ISL29125)
 *
 * The library assumes that the i2c bus has been initialized already.
 * It will only communicate by issuing start/stop conditions and transmitting
 * command and data requests.
 *
 * The 36 bit color mode is supported. However, downsampling it to 24 bit will
 * incur an extra cost of reading the color mode register (i2c transmission) for
 * each color.
 *
 * @author Matthias L. Jugel
 *
 * == LICENSE ==
 * Copyright 2015 ubirch GmbH (http://www.ubirch.com)
 *
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

#include <drivers/fsl_i2c.h>
#include <utilities/fsl_debug_console.h>
#include "isl29125.h"
#include "../libs/i2c/i2c_core.h"


void isl_set(uint8_t reg, uint8_t data) {
  i2c_write_reg(ISL_DEVICE_ADDRESS, reg, &data, 1);
}

uint8_t isl_get(uint8_t reg) {
  return i2c_read_reg(ISL_DEVICE_ADDRESS, reg);
}

uint8_t isl_reset(void) {
  // check device is there
  uint8_t device_id = i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_DEVICE_ID);
  PRINTF("device id: 0x%02x (should be 0x7d)\r\n", device_id);
  if (device_id != ISL_DEVICE_ID) return 0;

  // reset and make sure we are actually done resetting
  uint8_t reset = ISL_R_RESET;
  status_t status = i2c_write_reg(ISL_DEVICE_ADDRESS, 0x00, &reset, 1);
  I2C_MasterStop(I2C2);

  i2c_error("isl29125 reset", status);
  // maybe not necessary if correct stop signal is sent above
  uint8_t check, timeout = 5 /* do the check for 5 times */;
  do {
    check = 0x00;
    check |= i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_COLOR_MODE);
    check |= i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_FILTERING);
    check |= i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_INTERRUPT);
    check |= i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_STATUS);
  } while (check && --timeout);
  if (check) return 0;

  return 1;
}

uint16_t isl_read_red(void) {
  return i2c_read_reg16(ISL_DEVICE_ADDRESS, ISL_R_RED_L);
}

uint16_t isl_read_green(void) {
  return i2c_read_reg16(ISL_DEVICE_ADDRESS, ISL_R_GREEN_L);
}

uint16_t isl_read_blue(void) {
  return i2c_read_reg16(ISL_DEVICE_ADDRESS, ISL_R_BLUE_L);
}

static inline uint8_t downsample(uint16_t c) {
  const int shift = (i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_COLOR_MODE) & ISL_MODE_12BIT) ? 4 : 8;
  return (uint8_t) (0xff & c >> shift);
}

uint8_t isl_read_red8(void) {
  return downsample(isl_read_red());
}

uint8_t isl_read_green8(void) {
  return downsample(isl_read_green());;
}

uint8_t isl_read_blue8(void) {
  return downsample(isl_read_blue());
}

void isl_read_rgb48(rgb48_t *rgb48) {
  rgb48->red = isl_read_red(),
  rgb48->green = isl_read_green(),
  rgb48->blue = isl_read_blue();
}

void isl_read_rgb24(rgb24_t *rgb24) {
  rgb24->red = isl_read_red8(),
  rgb24->green = isl_read_green8(),
  rgb24->blue = isl_read_blue8();
}
