/**
 * Simple I2C bus scan.
 * - Initialize I2C2 bus
 * -
 */

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <extpin.h>
#include <drivers/fsl_lpuart.h>

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  LED_Write((counter % 100) < 10);
}

int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);

  // enable clocks and pins for I2C2 (Pin 4 and 5)
  CLOCK_EnableClock(kCLOCK_PortB);
  PORT_SetPinMux(PORTB, 10, kPORT_MuxAlt4);
  PORT_SetPinMux(PORTB, 11, kPORT_MuxAlt4);

  // initialize I2C
  i2c_master_transfer_t master_transfer;
  i2c_master_config_t i2c_config;
  I2C_MasterGetDefaultConfig(&i2c_config);

  // configure I2C
  I2C_MasterGetDefaultConfig(&i2c_config);
  i2c_config.baudRate_Bps = 400000U;
  I2C_MasterInit(I2C2, &i2c_config, CLOCK_GetFreq(kCLOCK_BusClk));

  memset(&master_transfer, 0, sizeof(master_transfer));

  // Do a transfer and stop for each of the addresses possible
  // output found device addresses or errors
  for (uint8_t address = 0x00; address <= 0x7f; address++) {
    master_transfer.slaveAddress = address;
    master_transfer.direction = kI2C_Write;
    master_transfer.subaddress = 0;
    master_transfer.subaddressSize = 0;
    master_transfer.data;
    master_transfer.dataSize = 0;
    master_transfer.flags = kI2C_TransferDefaultFlag;

    PRINTF("Scanning for device 0x%02x: \e[K", address);

    status_t status = I2C_MasterTransferBlocking(I2C2, &master_transfer);
    switch (status) {
      case kStatus_Success:
        PRINTF("FOUND\r\n");
            break;
      case kStatus_I2C_Nak:
        PRINTF("NOT FOUND\r");
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
        PRINTF("ERROR %0d\r", status);
            break;
    }
    I2C_MasterStop(I2C2);
  }

  PRINTF("\e[KScanning done.\r\n");

  return 0;
}
