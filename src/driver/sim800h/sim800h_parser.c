/**
 * @brief ubirch#1 SIM800H AT command parser.
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
#include <string.h>
#include "sim800h_core.h"
#include "sim800h_parser.h"

#ifndef NDEBUG

#include <utilities/fsl_debug_console.h>
#include <stdarg.h>
#include <stdio.h>

#else
# undef PRINTF
# define PRINTF(...)
#endif

int check_urc(const char *line) {
  size_t len = strlen(line);
  for (int i = 0; SIM800H_URC[i] != NULL; i++) {
    const char *urc = SIM800H_URC[i];
    size_t urc_len = strlen(SIM800H_URC[i]);
    if (len >= urc_len && !strncmp(urc, line, urc_len)) {
      PRINTF("GSM INFO !! [%02d] %s\r\n", i, line);
      return i;
    }
  }
  return -1;
}

void sim800h_send(const char *pattern, ...) {
  char cmd[255];
  va_list ap;
  va_start(ap, pattern);
  vsnprintf(cmd, 254, pattern, ap);
  va_end(ap);

  // cleanup the input buffer
  while(sim800h_read() != -1);
  PRINTF("GSM (%02d) <- '%s'\r\n", strlen(cmd), cmd);
  sim800h_writeline(cmd);
}

bool sim800h_expect_urc(int n, uint32_t timeout) {
  char response[128] = {0};
  bool urc_found = false;
  do {
    const size_t len = sim800h_readline(response, CELL_PARSER_BUFSIZE, timeout);
    if (!len) break;
    int r = check_urc(response);
    urc_found = r == n;
#ifndef NDEBUG
    if(r == 0 && !urc_found) {
      PRINTF("GSM .... ?? ");
      for(int i = 0; i < len; i++) PRINTF("%02x '%c' ", *(response+i), *(response+i));
      PRINTF("\r\n");
    }
#endif
  } while(!urc_found);
  return urc_found;
}

bool sim800h_expect(const char *expected, uint32_t timeout) {
  char response[255] = {0};
  size_t len, expected_len = strlen(expected);
  while (true) {
    len = sim800h_readline(response, CELL_PARSER_BUFSIZE, timeout);
    if(len == 0) return false;
    if (check_urc(response) >= 0) continue;
    PRINTF("GSM (%02d) -> '%s'\r\n", len, response);
    return strncmp(expected, (const char *) response, MIN(len, expected_len)) == 0;
  }
}

int sim800h_expect_scan(const char *pattern, uint32_t timeout, ...) {
  char response[CELL_PARSER_BUFSIZE];
  va_list ap;
  size_t len;
  do {
    len = sim800h_readline(response, CELL_PARSER_BUFSIZE-1, timeout);
  } while(check_urc(response) != -1);
  PRINTF("GSM (%02d) -> '%s'\r\n", len, response);

  va_start(ap, timeout);
  int matched = vsscanf(response, pattern, ap);
  va_end(ap);

  return matched;
}

