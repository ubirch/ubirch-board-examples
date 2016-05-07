/**
 * ubirch#1 timer driver code.
 *
 * Driver for timer related tasks.
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
#ifndef _UBIRCH_TIMER_H
#define _UBIRCH_TIMER_H

#include <stdint.h>
#include <board.h>

/*!
 * @brief Initialize the timer.
 */
void timer_init(void);

/*!
 * @brief Read current timer value.
 *
 * Will initialialize the timer if it has not been initialized.
 *
 * @return the current timestamp
 */
uint32_t timer_read(void);

/*!
 * @brief Schedule a timer interrupt in the future.
 * @param a timestamp (timer_read() + delta interval)
 */
void timer_schedule(uint32_t timestamp);

/*!
 * @brief Delay execution for a certain amount of time.
 *
 * This function will try to go into a low power mode (__WFI())until the
 * end of the delay. It will schedule an interrupt in the future
 * and under optimal conditions only wake up when the interrupt
 * is triggered. If other events preempt this it checks the
 * time and continues.
 *
 * @param ms the milliseconds to delay execution
 */
static inline void delay(uint32_t ms) {
  uint32_t timestamp = timer_read() + ms * 1000;
  timer_schedule(timestamp);
  while (timer_read() < timestamp) { __WFI(); }
}

#endif // _UBIRCH_TIMER_H
