/**
 * ubirch#1 i2c driver code (blocking).
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

#include <drivers/fsl_i2c.h>
#include <drivers/fsl_port.h>
#include <utilities/fsl_debug_console.h>
#include "i2c.h"

void i2c_init(i2c_speed_t speed) {
  CLOCK_EnableClock(kCLOCK_PortB);
  PORT_SetPinMux(PORTB, 10, kPORT_MuxAlt4);
  PORT_SetPinMux(PORTB, 11, kPORT_MuxAlt4);

  // configure I2C
  i2c_master_config_t i2c_config;
  I2C_MasterGetDefaultConfig(&i2c_config);
  i2c_config.baudRate_Bps = speed;
  I2C_MasterInit(I2C2, &i2c_config, CLOCK_GetFreq(I2C2_CLK_SRC));
}

void i2c_deinit() {
  I2C_MasterDeinit(I2C2);
  CLOCK_DisableClock(kCLOCK_PortB);
}

status_t i2c_ping(uint8_t address) {
  i2c_master_transfer_t transfer;
  memset(&transfer, 0, sizeof(transfer));

  transfer.slaveAddress = address;
  transfer.direction = kI2C_Write;
  transfer.flags = kI2C_TransferDefaultFlag;
  status_t status = I2C_MasterTransferBlocking(I2C2, &transfer);
  I2C_MasterStop(I2C2);
  return status;
}

status_t i2c_write(uint8_t address, uint8_t reg, uint8_t *data, size_t size) {
  i2c_master_transfer_t transfer;
  memset(&transfer, 0, sizeof(transfer));

  transfer.slaveAddress = address;
  transfer.direction = kI2C_Write;
  transfer.subaddress = reg;
  transfer.subaddressSize = 1;
  transfer.data = data;
  transfer.dataSize = size;
  transfer.flags = kI2C_TransferDefaultFlag;

  status_t status = I2C_MasterTransferBlocking(I2C2, &transfer);
#ifndef NDEBUG
  if (status != kStatus_Success) {
    PRINTF("I2C write(%02d) <= %02x\r\n", reg, *data);
    i2c_error("write reg", status);
  }
#endif
  if (!size && status == kStatus_Success)
    status = I2C_MasterStop(I2C2);
  return status;
}

status_t i2c_read(uint8_t address, uint8_t reg, uint8_t *data, size_t size) {

  i2c_master_transfer_t transfer;
  memset(&transfer, 0, sizeof(transfer));

  transfer.slaveAddress = address;
  transfer.direction = kI2C_Read;
  transfer.subaddress = reg;
  transfer.subaddressSize = 1;
  transfer.data = data;
  transfer.dataSize = size;
  transfer.flags = kI2C_TransferDefaultFlag;
  status_t status = I2C_MasterTransferBlocking(I2C2, &transfer);

#ifndef NDEBUG
  if (status != kStatus_Success) {
    PRINTF("I2C read(%02d) => %d byte\r\n", reg, transfer.dataSize);
    i2c_error("read (address)", status);
    I2C_MasterStop(I2C2);
  }
#endif
  return status;
}

uint8_t i2c_read_reg(uint8_t address, uint8_t reg) {

  i2c_read(address, reg, &reg, 1);
  return reg;
}

uint16_t i2c_read_reg16(uint8_t address, uint8_t reg) {

  uint8_t data[2];
  i2c_read(address, reg, data, 2);
  uint16_t value = (data[1] << 8 | data[0]);
  return value;
}

#ifndef NDEBUG
void i2c_error(char *s, status_t status) {
  if (status == kStatus_Success) return;
  PRINTF("%s (%x): ", s, status);
  switch (status) {
    case kStatus_I2C_Nak:
      PRINTF("NAK\r\n", s);
          break;
    case kStatus_I2C_ArbitrationLost:
      PRINTF("ARBITRATION LOST\r\n");
          break;
    case kStatus_I2C_Busy:
      PRINTF("BUSY\r\n");
          break;
    case kStatus_I2C_Idle:
      PRINTF("IDLE\r\n");
          break;
    case kStatus_I2C_Timeout:
      PRINTF("TIMEOUT\r\n");
          break;
    default:
      PRINTF("UNKNOWN ERROR\r\n", status);
          break;
  }
}
#endif
