#include <drivers/fsl_i2c.h>
#include <utilities/fsl_debug_console.h>
#include <drivers/fsl_port.h>

#include "i2c_core.h"

void i2c_init(i2c_speed_t speed) {
  CLOCK_EnableClock(kCLOCK_PortB);
  PORT_SetPinMux(PORTB, 10, kPORT_MuxAlt4);
  PORT_SetPinMux(PORTB, 11, kPORT_MuxAlt4);

  // configure I2C
  i2c_master_config_t i2c_config;
  I2C_MasterGetDefaultConfig(&i2c_config);
  i2c_config.baudRate_Bps = speed;
  I2C_MasterInit(I2C2, &i2c_config, CLOCK_GetFreq(kCLOCK_BusClk));
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

status_t i2c_write_reg(uint8_t address, uint8_t reg, uint8_t *data, size_t size) {
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
  if (status != kStatus_Success) {
    PRINTF("ISL29125 write(%02d) <= %02x\r\n", reg, *data);
    i2c_error("write reg", status);
  }
  if (!size && status == kStatus_Success)
    status = I2C_MasterStop(I2C2);
  return status;
}

uint8_t i2c_read_reg(uint8_t address, uint8_t reg) {

  i2c_master_transfer_t transfer;
  memset(&transfer, 0, sizeof(transfer));

  static uint8_t data = 0;
  transfer.slaveAddress = address;
  transfer.direction = kI2C_Read;
  transfer.subaddress = reg;
  transfer.subaddressSize = 1;
  transfer.data = &data;
  transfer.dataSize = 1;
  transfer.flags = kI2C_TransferDefaultFlag;
  status_t status = I2C_MasterTransferBlocking(I2C2, &transfer);
  if (status != kStatus_Success) {
    PRINTF("ISL29125 read(%02d) => %02x\r\n", reg, data);
    i2c_error("read reg", status);
  }
  return data;
}

uint16_t i2c_read_reg16(uint8_t address, uint8_t reg) {

  i2c_master_transfer_t transfer;
  memset(&transfer, 0, sizeof(transfer));

  static uint8_t data[2] = {0, 0};
  transfer.slaveAddress = address;
  transfer.direction = kI2C_Read;
  transfer.subaddress = reg;
  transfer.subaddressSize = 1;
  transfer.data = data;
  transfer.dataSize = 2;
  transfer.flags = kI2C_TransferDefaultFlag;
  status_t status = I2C_MasterTransferBlocking(I2C2, &transfer);

  uint16_t value = (data[1] << 8 | data[0]);
  if (status != kStatus_Success) {
    PRINTF("ISL29125 read(%02d) => %04x\r\n", reg, value);
    i2c_error("read reg16 (address)", status);
  }
  return value;
}

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
