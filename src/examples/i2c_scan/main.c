/**
 * I2C bus scan.
 *
 * Scans the bus by addressing each of the possible device ids.
 * Reports a device as found if it responds with an ACK. This
 * is useful if we have a new device and want to check if it
 * is there and active.
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

#include <stdint.h>
#include <board.h>
#include <stdio.h>
#include <i2c.h>

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  LED_Write((counter % 100) < 10);
}

int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);

  i2c_init(I2C_FULL_SPEED);

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

  i2c_deinit();

  PRINTF("\e[KScanning done.\r\n");

  return 0;
}
