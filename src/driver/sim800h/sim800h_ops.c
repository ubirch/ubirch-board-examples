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
#include <fsl_rtc.h>
#include <stdlib.h>
#include "sim800h_parser.h"
#include "sim800h.h"

#ifndef NDEBUG
static const char *reg_status[6] = {
  "NOT SEARCHING",
  "HOME",
  "SEARCHING",
  "DENIED",
  "UNKNOWN",
  "ROAMING"
};
#endif

bool sim800h_register(uint32_t timeout) {
  uint32_t us_target = timer_read() + timeout * 1000;
  timer_schedule(us_target);

  bool registered = false;
  while (!registered && (timer_read() < us_target)) {
    int x, status;
    sim800h_send("AT+CREG?");
    const int matched = sim800h_expect_scan("+CREG: %d,%d", us_target - timer_read(), &x, &status);
    sim800h_expect("OK", 500);
    if (matched == 2) {
      PRINTF("GSM INFO !! [%02d] %s\r\n", status, status < 6 ? reg_status[status] : "???");
      registered = ((status == CREG_HOME || status == CREG_ROAMING));
    }
    if (!registered) delay(2000);
  }

  return registered;
}

bool sim800h_gprs_attach(const char *apn, const char *user, const char *password, uint32_t timeout) {
  uint32_t us_target = timer_read() + timeout * 1000;
  timer_schedule(us_target);

  // shut down any previous GPRS connection
  sim800h_send("AT+CIPSHUT");
  if (!sim800h_expect("SHUT OK", us_target - timer_read())) return false;

  // enable multiplex mode (TODO check it necessary, I read somewhere multiplex mode is more stable)
  sim800h_send("AT+CIPMUX=1");
  if (!sim800h_expect("OK", us_target - timer_read())) return false;

  // enable manual receive mode
  sim800h_send("AT+CIPRXGET=1");
  if (!sim800h_expect("OK", us_target - timer_read())) return false;

  // attach to the network
  bool attached = false;
  do {
    sim800h_send("AT+CGATT=1");
    attached = sim800h_expect("OK", us_target - timer_read());
    if (!attached) delay(1000);
  } while (!attached && (timer_read() < us_target));
  if (!attached) return false;

  // configure connection
  sim800h_send("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  if (!sim800h_expect("OK", us_target - timer_read())) return false;

  // set bearer profile access point name
  if (apn) {
    sim800h_send("AT+SAPBR=3,1,\"APN\",\"%s\"", apn);
    if (!sim800h_expect("OK", us_target - timer_read())) return false;
    if (user) {
      sim800h_send("AT+SAPBR=3,1,\"USER\",\"%s\"", user);
      if (!sim800h_expect("OK", us_target - timer_read())) return false;
    }
    if (password) {
      sim800h_send("AT+SAPBR=3,1,\"PWD\",\"%s\"", password);
      if (!sim800h_expect("OK", us_target - timer_read())) return false;
    }
  }


  // open GPRS context
  sim800h_send("AT+SAPBR=1,1");
  sim800h_expect("OK", us_target - timer_read());

  int opened;
  do {
    sim800h_send("AT+SAPBR=2,1");
    if (!sim800h_expect_scan("+SAPBR: 1,%d", us_target - timer_read(), &opened)) return false;
  } while (opened != 1 && (timer_read() < us_target));

  return attached;
}

bool sim800h_battery(short int *status, short int *level, int *voltage, uint32_t timeout) {
  sim800h_send("AT+CBC");
  sim800h_expect_scan("+CBC: %d,%d,%d", timeout, status, level, voltage);
  return sim800h_expect("OK", 500);
}

bool sim800h_location(short int *status, double *lat, double *lon, rtc_datetime_t *datetime, uint32_t timeout) {
  char response[60];

  sim800h_send("AT+CIPGSMLOC=1,1");
  sim800h_expect_scan("+CIPGSMLOC: %d,%s", timeout, &status, response);

  *lon = atof(strtok(response, ","));
  *lat = atof(strtok(NULL, ","));
  datetime->year = (uint16_t) atoi(strtok(NULL, "/"));
  datetime->month  = (uint8_t) atoi(strtok(NULL, "/"));
  datetime->day = (uint8_t) atoi(strtok(NULL, ","));
  datetime->hour  = (uint8_t) atoi(strtok(NULL, ":"));
  datetime->minute = (uint8_t) atoi(strtok(NULL, ":"));
  datetime->second = (uint8_t) atoi(strtok(NULL, ":"));

  return sim800h_expect("OK", 500);
}

bool sim800h_imei(char *imei) {
  sim800h_send("AT+GSN");
  sim800h_readline(imei, 16, 1000);
  return sim800h_expect("OK", 1000);
}
