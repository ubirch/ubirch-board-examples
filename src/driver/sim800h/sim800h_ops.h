/**
 * ubirch#1 SIM800H high level operations.
 *
 * @author Matthias L. Jugel
 * @date 2016-05-06
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

#ifndef _UBIRCH_SIM800H_OPS_H_
#define _UBIRCH_SIM800H_OPS_H_

#include <stdbool.h>
#include <stdint.h>
#include <fsl_rtc.h>

/*!
 * @brief Register to the cell network.
 * @param timeout how long to wait until registration
 */
bool sim800h_register(uint32_t timeout);

bool sim800h_gprs_attach(const char *apn, const char *user, const char *password, uint32_t timeout);

bool sim800h_battery(short int *status, short int *level, int *voltage, uint32_t timeout);

bool sim800h_location(short int *status, double *lat, double *lon, rtc_datetime_t *datetime, uint32_t timeout);

#endif // _UBIRCH_SIM800H_OPS_H_
