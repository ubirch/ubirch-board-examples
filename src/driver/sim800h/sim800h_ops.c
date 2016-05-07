/**
 * ubirch#1 SIM800H AT command parser.
 *
 * @author Matthias L. Jugel
 * @date 2016-04-09
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

#include <timer.h>
#include <stdbool.h>
#include <fsl_debug_console.h>
#include "sim800h_parser.h"

bool sim800h_register(uint32_t timeout) {
  uint32_t start = timer_read();
  while ((timer_read() - start) < (timeout * 1000)) {
    int status;
    sim800h_send("AT+CREG?");

    if (sim800h_expect_scan("+CREG %d,%d", (timeout - (timer_read() - start)), &status)) {
      switch (status) {
        case CREG_NOT_SEARCHING:
          PRINTF("_");
              break;
        case CREG_HOME:
          PRINTF("H");
              break;
        case CREG_SEARCHING:
          PRINTF("S");
              break;
        case CREG_DENIED:
          PRINTF("D");
              break;
        case CREG_UNKNOWN:
          PRINTF("?");
              break;
        case CREG_ROAMING:
          PRINTF("R");
              break;
        default:
          PRINTF("unknown: %d", status);
              break;
      }
      if ((status == CREG_HOME || status == CREG_ROAMING)) {
        PRINTF("\r\n");
        return true;
      }
    }
  }

  return false;
}
