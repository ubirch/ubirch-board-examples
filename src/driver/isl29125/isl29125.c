/**
 * Driver for the RGB sensor ISL29125.
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

#include <utilities/fsl_debug_console.h>
#include <drivers/fsl_i2c.h>
#include <i2c.h>
#include "isl29125.h"

void isl_set(uint8_t reg, uint8_t data) {
  i2c_write(ISL_DEVICE_ADDRESS, reg, &data, 1);
}

uint8_t isl_get(uint8_t reg) {
  return i2c_read_reg(ISL_DEVICE_ADDRESS, reg);
}

uint8_t isl_reset(void) {
  // check device is there
  uint8_t device_id = i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_DEVICE_ID);
  if (device_id != ISL_DEVICE_ID) {
#ifndef NDEBUG
    PRINTF("device id: 0x%02x (should be 0x7d)\r\n", device_id);
#endif
    return 0;
  }

  // reset and make sure we are actually done resetting
  uint8_t reset = ISL_RESET;
  status_t status = i2c_write(ISL_DEVICE_ADDRESS, 0x00, &reset, 1);
  I2C_MasterStop(I2C2);

#ifndef NDEBUG
  i2c_error("isl_reset()", status);
#endif
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

// TODO: burst read 48 bit
void isl_read_rgb48(rgb48_t *rgb48) {
  rgb48->red = i2c_read_reg16(ISL_DEVICE_ADDRESS, ISL_R_RED_L);
  rgb48->green = i2c_read_reg16(ISL_DEVICE_ADDRESS, ISL_R_GREEN_L);
  rgb48->blue = i2c_read_reg16(ISL_DEVICE_ADDRESS, ISL_R_BLUE_L);
//  rgb48->red = (i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_RED_H) << 8) + i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_RED_L);
//  rgb48->green = (i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_GREEN_H) << 8) + i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_GREEN_L);
//  rgb48->blue = (i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_BLUE_H) << 8) + i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_BLUE_L);

}

void isl_read_rgb24(rgb24_t *rgb24) {
  uint8_t data[6];
  i2c_read(ISL_DEVICE_ADDRESS, ISL_R_GREEN_L, data, 6);
  const int shift = (i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_COLOR_MODE) & ISL_MODE_12BIT) ? 4 : 8;

  rgb24->red = (data[4] << 8 | data[5]) >> shift;
  rgb24->green = (data[2] << 8 | data[3]) >> shift;
  rgb24->blue = (data[0] << 8 | data[1]);
}
