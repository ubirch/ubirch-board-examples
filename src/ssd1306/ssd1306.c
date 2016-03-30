/**
 * SSD1306 driver.
 *
 * @author Matthias L. Jugel
 *
 * Copyright 2015 ubirch GmbH (http://www.ubirch.com)
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

#include <extpin.h>
#include <board.h>
#include "ssd1306.h"
#include "../libs/i2c/i2c_core.h"

// == low level functions ==================================

void oled_reset(EXTPIN_T reset_pin) {
  ExtPin_SetOutput(reset_pin);
  ExtPin_Write(reset_pin, true);
  BusyWait100us(1);
  ExtPin_Write(reset_pin, false);
  BusyWait100us(1);
  ExtPin_Write(reset_pin, true);
}

void oled_cmd(uint8_t address, uint8_t command) {
  i2c_write_reg(address, 0x00, &command, 1);
}


void oled_data(uint8_t address, uint8_t data) {
  i2c_write_reg(address, 0x40, &data, 1);
}

//== higher level functions ================================

void oled_clear(uint8_t address) {
  oled_cmd(address, 0x21);
  oled_cmd(address, 0);
  oled_cmd(address, 127);

  oled_cmd(address, 0x22);
  oled_cmd(address, 0);
  oled_cmd(address, 7);

  uint8_t cleared_buffer[128];
  memset(cleared_buffer, 0, 128);
  for (uint8_t page = 0; page < 8; page++) {
    oled_cmd(address, (uint8_t) 0xb0 | page);
    oled_cmd(address, 0x00);
    i2c_write_reg(address, 0x40, cleared_buffer, 128);
  }
}

