/**
 * RGB Sensor test (ISL29125).
 *
 * Samples colors from the RGB sensor and displays them in 0.5s intervals.
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
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <isl29125.h>
#include <i2c.h>

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  LED_Write((counter % 100) < 10);
}


int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);
  PRINTF("\r\n-- ISL29125 test\r\n");

  i2c_init(I2C_STANDARD);

  if (!isl_reset()) {
    PRINTF("could not initialize ISL29125 RGB sensor\r\n");
  }

  // set sampling mode, ir filter and interrupt mode
  isl_set(ISL_R_COLOR_MODE, ISL_MODE_RGB | ISL_MODE_10KLUX | ISL_MODE_16BIT);
  isl_set(ISL_R_FILTERING, ISL_FILTER_IR_NONE);

  uint8_t color_mode = isl_get(ISL_R_COLOR_MODE);
  uint8_t filter_mode = isl_get(ISL_R_FILTERING);
  uint8_t intr = isl_get(ISL_R_INTERRUPT);
  PRINTF("read mode      : 0x%02x (%08b)\r\n", color_mode, color_mode);
  PRINTF("read filter    : 0x%02x (%08b)\r\n", filter_mode, filter_mode);
  PRINTF("read interrupts: 0x%02x (%08b)\r\n", intr, intr);

  PRINTF("reading RGB values from sensor\r\n");
  PRINTF("'%%' indicates the chip is still in a conversion cyle, so we wait\r\n");
  while (true) {
    // wait for the conversion cycle to be done, this just indicates there is a cycle
    // in progress. the actual r,g,b values are always available from the last cycle
    for (uint8_t colors = 0; colors < 3; colors++) {
      uint8_t timeout = 150;
      while (!(isl_get(ISL_R_STATUS) & ISL_STATUS_ADC_DONE) && --timeout) BusyWait100us(1);
      PUTCHAR('%');
    }
    PRINTF("\r\n");

    // read the full 36 or 48 bit color
    PRINTF("48bit: ");
    rgb48_t rgb48;
    isl_read_rgb48(&rgb48);
    PRINTF("0x%04x%04x%04x rgb48(%u,%u,%u)\r\n", rgb48.red, rgb48.green, rgb48.blue, rgb48.red, rgb48.green, rgb48.blue);

    PRINTF("24bit: ");
    rgb24_t rgb24;
    isl_read_rgb24(&rgb24);
    PRINTF("0x%02x%02x%02x rgb24(%u,%u,%u)\r\n", rgb24.red, rgb24.green, rgb24.blue, rgb24.red, rgb24.green, rgb24.blue);

    BusyWait100us(25000);
  }

  I2C_MasterDeinit(I2C2);

  return 0;

}
