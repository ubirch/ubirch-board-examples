/**
 * Timer Test.
 *
 * @author Matthias L. Jugel
 * @date 2016-04-06
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
#include <stdio.h>
#include <board.h>
#include <ubirch/timer.h>

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
#ifdef BOARD_LED0
  BOARD_LED0((counter % 100) < 10);
#endif
#ifdef BOARD_LED1
  BOARD_LED1((counter % 200) < 50);
#endif
#ifdef BOARD_LED2
  BOARD_LED2((counter % 500) < 100);
#endif
}

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  PRINTF("BOARD TEST\r\n");
  SysTick_Config(BOARD_SYSTICK_100MS);

  while(1) {
    uint32_t start = timer_read();
    delay(10000);
    uint32_t end = timer_read();

    PRINTF("%lu-%lu = %lu\r\n", end, start, (end-start) / 1000);
  }

}
