/**
 * @brief ubirch#1 SIM800H debug helpers.
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

#ifndef _UBIRCH_SIM800H_DEBUG_
#define _UBIRCH_SIM800H_DEBUG_

#ifndef NDEBUG

#include <fsl_debug_console.h>
#define CIODEBUG(...)  PRINTF(__VA_ARGS__)
#define CSTDEBUG(...)  PRINTF(__VA_ARGS__)

#ifdef NCIODEBUG
#  undef CIODEBUG
#  define CIODEBUG(...)
#endif
#ifdef NCSTDEBUG
#  undef CIODEBUG
#  define CSTDEBUG(...)
#endif

const char *reg_status[6] = {
  "NOT SEARCHING",
  "HOME",
  "SEARCHING",
  "DENIED",
  "UNKNOWN",
  "ROAMING"
};

#endif

#endif // _UBIRCH_SIM800H_DEBUG_
