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
#include "isl29125.h"

#ifndef NDEBUG
#   ifdef i2c_assert
#      undef i2c_assert
#   endif
#   define i2c_assert(e, m)
#endif

bool i2c_write_reg(uint8_t address, uint8_t reg, uint8_t data) {
  uint8_t buffer[2] = { reg, data };

  I2C_MasterStart(I2C2, 0, kI2C_Write);
  i2c_master_transfer_t transfer;
  transfer.slaveAddress = address;
  transfer.direction = kI2C_Write;
  transfer.subaddress = 0;
  transfer.subaddressSize = 0;
  transfer.data = buffer;
  transfer.dataSize = 1;
  transfer.flags = kI2C_TransferDefaultFlag;
  status_t status = I2C_MasterTransferBlocking(I2C2, &transfer);
  return (status == kStatus_Success);
}

uint8_t i2c_read_reg(uint8_t address, uint8_t reg) {
  I2C_MasterStart(I2C2, 0, kI2C_Write);
  i2c_master_transfer_t transfer;
  transfer.slaveAddress = address;
  transfer.direction = kI2C_Write;
  transfer.subaddress = 0;
  transfer.subaddressSize = 0;
  transfer.data = &reg;
  transfer.dataSize = 1;
  transfer.flags = kI2C_TransferDefaultFlag;
  status_t status = I2C_MasterTransferBlocking(I2C2, &transfer);

  I2C_MasterStart(I2C2, 0, kI2C_Write);
  uint8_t data[1];
  transfer.slaveAddress = address;
  transfer.direction = kI2C_Read;
  transfer.subaddress = 0;
  transfer.subaddressSize = 0;
  transfer.data = data;
  transfer.dataSize = 1;
  transfer.flags = kI2C_TransferDefaultFlag;
  status = I2C_MasterTransferBlocking(I2C2, &transfer);

  return data[0];
}

void isl_set(uint8_t reg, uint8_t data) {
  i2c_write_reg(ISL_DEVICE_ADDRESS, reg, data);
}

uint8_t isl_get(uint8_t reg) {
  return i2c_read_reg(ISL_DEVICE_ADDRESS, reg);
}

uint8_t isl_reset(void) {
  I2C_MasterStart(I2C2, ISL_DEVICE_ADDRESS, kI2C_Read);
  i2c_write(ISL_DEVICE_ADDRESS << 1);
  i2c_assert(I2C_STATUS_SLAW_ACK, "address error");
  i2c_write(ISL_R_RESET);
  i2c_assert(I2C_STATUS_DATA_ACK, "register error");
  i2c_stop();

  if (i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_DEVICE_ID) != ISL_DEVICE_ID) return 0;

  uint8_t check = 0x00;
  check |= i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_COLOR_MODE);
  check |= i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_FILTERING);
  check |= i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_INTERRUPT);
  check |= i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_STATUS);
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
  return c >> ((i2c_read_reg(ISL_DEVICE_ADDRESS, ISL_R_COLOR_MODE) & ISL_MODE_12BIT) ? 4 : 8);
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

rgb48 isl_read_rgb(void) {
  rgb48 result = {
    .red = isl_read_red(),
    .green = isl_read_green(),
    .blue = isl_read_blue()
  };
  return result;
}

rgb24 isl_read_rgb24(void) {
  rgb24 result = {
    .red = isl_read_red8(),
    .green = isl_read_green8(),
    .blue = isl_read_blue8()
  };
  return result;
}
