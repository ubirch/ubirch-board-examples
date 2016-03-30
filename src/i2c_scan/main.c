/**
 * Simple I2C bus scan.
 */

#include <stdint.h>
#include <board.h>
#include <stdio.h>
#include "../libs/i2c/i2c_core.h"

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  LED_Write((counter % 100) < 10);
}

int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);

  i2c_init(400000U);

  // Do a transfer and stop for each of the addresses possible
  // output found device addresses or errors
  for (uint8_t address = 0x00; address <= 0x7f; address++) {
    PRINTF("Scanning for device 0x%02x: \e[K", address);

    status_t status = i2c_ping(address);
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
  }

  I2C_MasterDeinit(I2C2);

  PRINTF("\e[KScanning done.\r\n");

  return 0;
}
