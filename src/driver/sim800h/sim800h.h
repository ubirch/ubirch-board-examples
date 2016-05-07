/**
 * ubirch#1 SIM800H cell phone chip driver.
 *
 * This is the SIM800H driver for the cell phone chip on the ubirch#1.
 * As the SIM800H is hard-wired to the LPUART1 all the configuration
 * is wrapped away from the user. This driver provides the functionality
 * to power on/off the chip and has low-level functionality for
 * communicating with the SIM800H using the AT command set.
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
#ifndef UBIRCH_SIM800_H
#define UBIRCH_SIM800_H

#include <stddef.h>
#include <stdint.h>

/*!
 * Enable the power domain for the SIM800H.
 * Returns as soon as the voltage is high enough to make the chip work.
 */
void sim800h_enable();

/*! Disable the power domain for the SIM800H */
void sim800h_disable();
/*!
 * Enable communication with the SIM800H.
 * This actually configures the UART and powers/resets the chip to
 * start a fresh cycle.
 */
void sim800h_init();

/*!
 * Read a single byte from the ringbuffer.
 * @return character or -1 if no data available
 */
int sim800h_read();

/*! Read a single line from the SIM800H */
size_t sim800h_readline(char *buffer, size_t max, uint32_t timeout);

/*! Write a line to the SIM800H and terminate it using CR LF */
void sim800h_writeline(const char *buffer);

#endif
