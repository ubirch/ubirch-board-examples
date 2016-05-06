/**
 * ubirch#1 timer driver code.
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
#include <fsl_pit.h>
#include "timer.h"

static bool initialized = false;

void PIT3_IRQHandler() {
  PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, PIT_TFLG_TIF_MASK);
  __SEV();
}

void timer_init() {
  pit_config_t pitConfig;
  PIT_GetDefaultConfig(&pitConfig);
  PIT_Init(PIT, &pitConfig);

  PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, (uint32_t) USEC_TO_COUNT(1U, CLOCK_GetFreq(kCLOCK_BusClk)));
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, 0xFFFFFFFF);
  PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, true);

  PIT_StartTimer(PIT, kPIT_Chnl_0);
  PIT_StartTimer(PIT, kPIT_Chnl_1);

  PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, (uint32_t) USEC_TO_COUNT(1U, CLOCK_GetFreq(kCLOCK_BusClk)));
  PIT_SetTimerPeriod(PIT, kPIT_Chnl_3, 0xFFFFFFFF);
  PIT_EnableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
  EnableIRQ(PIT3_IRQn);
}

uint32_t timer_read() {
  if (!initialized) timer_init();
  return ~(PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_1));
}
