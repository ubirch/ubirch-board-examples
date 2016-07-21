/**
 *
 *
 * @author Matthias L. Jugel
 * @date 2016-07-07
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
#include <stdbool.h>
#include <stdio.h>
#include <board.h>
#include <ubirch/timer.h>
#include <fsl_ftm.h>

#define BOARD_FTM_BASEADDR FTM3
#define BOARD_FIRST_FTM_CHANNEL 4U
#define BOARD_SECOND_FTM_CHANNEL 5U

#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)


int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  /* Initialize FTM3 pins below */
  /* Affects PORTC_PCR8 register */
  PORT_SetPinMux(PORTC, 8U, kPORT_MuxAlt3);
  /* Affects PORTC_PCR9 register */
  PORT_SetPinMux(PORTC, 9U, kPORT_MuxAlt3);

  bool brightnessUp = true; /* Indicate LEDs are brighter or dimmer */
  ftm_config_t ftmInfo;
  uint8_t updatedDutycycle = 100U;
  ftm_chnl_pwm_signal_param_t ftmParam[2];

  /* Configure ftm params with frequency 24kHZ */
  ftmParam[0].chnlNumber = (ftm_chnl_t)BOARD_SECOND_FTM_CHANNEL;
  ftmParam[0].level = kFTM_LowTrue;
  ftmParam[0].dutyCyclePercent = 0U;
  ftmParam[0].firstEdgeDelayPercent = 0U;

//  ftmParam[1].chnlNumber = (ftm_chnl_t)BOARD_SECOND_FTM_CHANNEL;
//  ftmParam[1].level = kFTM_LowTrue;
//  ftmParam[1].dutyCyclePercent = 0U;
//  ftmParam[1].firstEdgeDelayPercent = 0U;
//
  FTM_GetDefaultConfig(&ftmInfo);
  /* Initialize FTM module */
  FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);

  FTM_SetupPwm(BOARD_FTM_BASEADDR, ftmParam, 1U, kFTM_EdgeAlignedPwm, 44100U, FTM_SOURCE_CLOCK);
  FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);

  FTM_UpdatePwmDutycycle(BOARD_FTM_BASEADDR, (ftm_chnl_t)BOARD_SECOND_FTM_CHANNEL, kFTM_EdgeAlignedPwm, 0 /* percent */);

  while (1)
  {
    /* Delay to see the change of LEDs brightness */
    delay(100);

    if (brightnessUp)
    {
      /* Increase duty cycle until it reach limited value */
      if (++updatedDutycycle == 100U)
      {
        brightnessUp = false;
      }
    }
    else
    {
      /* Decrease duty cycle until it reach limited value */
      if (--updatedDutycycle == 0U)
      {
        brightnessUp = true;
      }
    }
    /* Start PWM mode with updated duty cycle */
    FTM_UpdatePwmDutycycle(BOARD_FTM_BASEADDR, (ftm_chnl_t)BOARD_FIRST_FTM_CHANNEL, kFTM_EdgeAlignedPwm,
                           updatedDutycycle);
    FTM_UpdatePwmDutycycle(BOARD_FTM_BASEADDR, (ftm_chnl_t)BOARD_SECOND_FTM_CHANNEL, kFTM_EdgeAlignedPwm,
                           updatedDutycycle);
    /* Software trigger to update registers */
    FTM_SetSoftwareTrigger(BOARD_FTM_BASEADDR, true);
  }

  PRINTF("DEBUG CONSOLE\r\n");
  while (true) {
    int ch = GETCHAR();
    if (ch == '\r') PUTCHAR('\n');
    PUTCHAR(ch);
  }
}
