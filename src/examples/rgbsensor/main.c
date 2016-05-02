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
#include <stdio.h>
#include <isl29125.h>
#include <i2c.h>
#include <utilities/fsl_debug_console.h>
#include <board.h>

#define ISL_327LUX_MAX 65000
#define ISL_10KLUX_MIN 8000

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  BOARD_LED0((counter % 100) < 10);
}

void sample_rgb(rgb48_t *color, uint8_t color_mode) {
  // set sampling mode, ir filter and interrupt mode
  isl_set(ISL_R_FILTERING, ISL_FILTER_IR_MAX);
  isl_set(ISL_R_COLOR_MODE, (uint8_t) (ISL_MODE_RGB | color_mode | ISL_MODE_16BIT));

  // give the device time to sample (2 fill RGB cycles (100ms per color))
  // well, theoretically, fact is, lowering the delay to 400ms will make it
  // almost always miss blue
  delay(600);

  isl_read_rgb48(color);
}

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(SystemCoreClock / 100U);
  PRINTF("\r\n-- ISL29125 test\r\n");

  i2c_init(I2C_FULL_SPEED);

  if (isl_reset()) PRINTF("could not initialize ISL29125 RGB sensor\r\n");

  // set sampling mode, ir filter and interrupt mode
  isl_set(ISL_R_COLOR_MODE, ISL_MODE_RGB | ISL_MODE_375LUX | ISL_MODE_16BIT);
  isl_set(ISL_R_FILTERING, ISL_FILTER_IR_NONE);

  uint8_t color_mode = isl_get(ISL_R_COLOR_MODE);
  uint8_t filter_mode = isl_get(ISL_R_FILTERING);
  uint8_t intr = isl_get(ISL_R_INTERRUPT);
  PRINTF("read mode      : 0x%02x (%08b)\r\n", color_mode, color_mode);
  PRINTF("read filter    : 0x%02x (%08b)\r\n", filter_mode, filter_mode);
  PRINTF("read interrupts: 0x%02x (%08b)\r\n", intr, intr);

  PRINTF("reading RGB values from sensor\r\n");
  PRINTF("'%%' indicates the chip is still in a conversion cyle, so we wait\r\n");
  uint8_t sensitivity = ISL_MODE_375LUX;
  while (true) {
    rgb48_t rgb48;

    if (!isl_reset()) {
      PRINTF("could not initialize ISL29125 RGB sensor\r\n");
    }

    // do an initial sampling
    sample_rgb(&rgb48, sensitivity);

    // auto-compensate for brightness
    if (sensitivity == ISL_MODE_375LUX &&
        rgb48.red > ISL_327LUX_MAX && rgb48.green > ISL_327LUX_MAX && rgb48.blue > ISL_327LUX_MAX) {
      sensitivity = ISL_MODE_10KLUX;
      sample_rgb(&rgb48, sensitivity);
    } else if (sensitivity == ISL_MODE_10KLUX &&
               rgb48.red < ISL_10KLUX_MIN && rgb48.green < ISL_10KLUX_MIN && rgb48.blue < ISL_10KLUX_MIN) {
      sensitivity = ISL_MODE_375LUX;
      sample_rgb(&rgb48, sensitivity);
    }

    PRINTF("\r%6s 0x%04x%04x%04x rgb48(%lu,%lu,%lu) rgb24(%u,%u,%u)\e[K",
           sensitivity == ISL_MODE_375LUX ? "375LUX" : "10KLUX",
           rgb48.red, rgb48.green, rgb48.blue,
           rgb48.red, rgb48.green, rgb48.blue,
           rgb48.red >> 8, rgb48.green >> 8, rgb48.blue >> 8);

    isl_set(ISL_R_COLOR_MODE, ISL_MODE_POWERDOWN);

    delay(100);
  }

  I2C_MasterDeinit(I2C2);

  return 0;

}
