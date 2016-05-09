/**
 * @brief ubirch#1 SIM800H high level functions.
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
#include <fsl_rtc.h>
#include <stdlib.h>
#include "sim800h_parser.h"
#include "sim800h_core.h"
#include "sim800h_debug.h"

#define REMAINING(t) (((t) - timer_read())/1000)

bool sim800h_register(const uint32_t timeout) {
  uint32_t us_target = timer_schedule_in(timeout * 1000);

  bool registered = false;
  while (!registered && (timer_read() < us_target)) {
    int bearer = 0, status = 0;
    sim800h_send("AT+CREG?");
    const int matched = sim800h_expect_scan("+CREG: %d,%d", REMAINING(us_target), &bearer, &status);
    sim800h_expect_OK(500);
    if (matched == 2) {
      CSTDEBUG("GSM INFO !! [%02d] %s\r\n", status, status < 6 ? reg_status[status] : "???");
      registered = ((status == CREG_HOME || status == CREG_ROAMING));
    }
    if (!registered) delay(2000);
  }

  return registered;
}

bool sim800h_gprs_attach(const char *apn, const char *user, const char *password, const uint32_t timeout) {
  uint32_t us_target = timer_schedule_in(timeout * 1000);

  // shut down any previous GPRS connection
  sim800h_send("AT+CIPSHUT");
  if (!sim800h_expect("SHUT OK", REMAINING(us_target))) return false;

  // enable multiplex mode (TODO check it necessary, I read somewhere multiplex mode is more stable)
  sim800h_send("AT+CIPMUX=1");
  if (!sim800h_expect_OK(REMAINING(us_target))) return false;

  // enable manual receive mode
  sim800h_send("AT+CIPRXGET=1");
  if (!sim800h_expect_OK(REMAINING(us_target))) return false;

  // attach to the network
  bool attached = false;
  do {
    sim800h_send("AT+CGATT=1");
    attached = sim800h_expect_OK(REMAINING(us_target));
    if (!attached) delay(1000);
  } while (!attached && (timer_read() < us_target));
  if (!attached) return false;

  // configure connection
  sim800h_send("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  if (!sim800h_expect_OK(REMAINING(us_target))) return false;

  // set bearer profile access point name
  if (apn) {
    sim800h_send("AT+SAPBR=3,1,\"APN\",\"%s\"", apn);
    if (!sim800h_expect_OK(REMAINING(us_target))) return false;
    if (user) {
      sim800h_send("AT+SAPBR=3,1,\"USER\",\"%s\"", user);
      if (!sim800h_expect_OK(REMAINING(us_target))) return false;
    }
    if (password) {
      sim800h_send("AT+SAPBR=3,1,\"PWD\",\"%s\"", password);
      if (!sim800h_expect_OK(REMAINING(us_target))) return false;
    }
  }

  // open GPRS context
  sim800h_send("AT+SAPBR=1,1");
  sim800h_expect_OK(REMAINING(us_target));

  int opened;
  do {
    sim800h_send("AT+SAPBR=2,1");
    sim800h_expect_scan("+SAPBR: 1,%d", REMAINING(us_target), &opened);
    if(!sim800h_expect_OK(REMAINING(us_target))) return false;
    delay(1000);
  } while (opened != 1 && (timer_read() < us_target));

  return attached;
}

bool sim800h_gprs_detach(uint32_t timeout) {
  uint32_t us_target = timer_schedule_in(timeout * 1000);

  sim800h_send("AT+CIPSHUT");
  if (!sim800h_expect("SHUT OK", REMAINING(us_target))) return false;

  sim800h_send("AT+SAPBR=0,1");
  if (!sim800h_expect_OK(REMAINING(us_target))) return false;

  sim800h_send("AT+CGATT=0");
  return sim800h_expect_OK(REMAINING(us_target));
}


bool sim800h_battery(status_t *status, short int *level, int *voltage, const uint32_t timeout) {
  sim800h_send("AT+CBC");
  sim800h_expect_scan("+CBC: %d,%d,%d", timeout, status, level, voltage);
  return sim800h_expect_OK(500);
}

bool sim800h_location(status_t *status, double *lat, double *lon, rtc_datetime_t *datetime, const uint32_t timeout) {
  char response[60];

  sim800h_send("AT+CIPGSMLOC=1,1");
  sim800h_expect_scan("+CIPGSMLOC: %d,%s", timeout, &status, response);

  if(status == 0) {
    *lon = atof(strtok(response, ","));
    *lat = atof(strtok(NULL, ","));

    datetime->year = (uint16_t) atoi(strtok(NULL, "/"));
    datetime->month = (uint8_t) atoi(strtok(NULL, "/"));
    datetime->day = (uint8_t) atoi(strtok(NULL, ","));
    datetime->hour = (uint8_t) atoi(strtok(NULL, ":"));
    datetime->minute = (uint8_t) atoi(strtok(NULL, ":"));
    datetime->second = (uint8_t) atoi(strtok(NULL, ":"));
  }

  return sim800h_expect_OK(500) && status == 0;
}

bool sim800h_imei(char *imei, const uint32_t timeout) {
  sim800h_send("AT+GSN");
  sim800h_readline(imei, 16, timeout);
  CIODEBUG("GSM (%02d) -> '%s'\r\n", strnlen(imei, 15), imei);
  return sim800h_expect_OK(500);
}
