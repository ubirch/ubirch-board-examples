/**
 * ubirch#1 SIM800H AT command parser.
 *
 * AT command driver, handling the I/O via the UART and parsing
 * lines for expected responses. Also handles so call unsolicited
 * response codes which are output at unexpected times.
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
#ifndef _SIM800H_PARSER_H_
#define _SIM800H_PARSER_H_

#include <stddef.h>
#include <stdbool.h>

/*!
 * A list of unsolicited response codes we need to take care of.
 * The list of by no means complete and may change for other versions
 * of the SIM800 chip line or other GSM chips.
 *
 * The list originates here: this useful list found here: https://github.com/cloudyourcar/attentive
 * You can also review http://simcom.ee/documents/SIM800H/SIM800%20Series_AT%20Command%20Manual_V1.09.pdf
 * Page 350+
 */
//
const char *SIM800H_URC[] = {
  "+CIPRXGET: 1,",  /*! incoming socket data notification */
  "+FTPGET: 1,",    /*! FTP state change notification */
  "+PDP: DEACT",    /*! PDP disconnected */
  "+SAPBR 1: DEACT",/*! PDP disconnected (for SAPBR apps) */
  "*PSNWID:",       /*! AT+CLTS network name */
  "*PSUTTZ:",       /*! AT+CLTS time */
  "+CTZV:",         /*! AT+CLTS timezone */
  "DST:",           /*! AT+CLTS dst information */
  "+CIEV:",         /*! AT+CLTS undocumented indicator */
  "RDY",            /*! Device ready. */
  "+CFUN: 1",       /*! Device has entered full functional mode */
  "+CPIN: READY",
  "Call Ready",
  "SMS Ready",
  "NORMAL POWER DOWN",
  "UNDER-VOLTAGE POWER DOWN",
  "UNDER-VOLTAGE WARNNING",
  "OVER-VOLTAGE POWER DOWN",
  "OVER-VOLTAGE WARNNING",
  NULL
};

/*!
 * Check if this line is an unsolicited result code.
 * @return the code index or -1 if it is no known code
 */
int check_urc(const char *line);

/*! send a command */
void sim800h_send(const char *cmd);

/*! expect a specific URC, blocks until it is received */
void sim800h_expect_urc(int n);

/*!
 * Expect a certain response, blocks util the response received
 * This function will ignore URCs and return when the first non-URC has been received.
 * @return true if received or false if not
 */
bool sim800h_expect(const char *expected);

#endif // _SIM800H_PARSER_H_
