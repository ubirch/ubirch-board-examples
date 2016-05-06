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

void timer_init(void);
uint32_t timer_read(void);

/*!
 * @brief Busy loop backed by timer and using __WFE().
 * @param ms the milliseconds to delay execution
 */
static inline void delay(uint32_t ms) {
  uint32_t us_interval = ms * 1000;
  uint32_t start = timer_read();
  while ((timer_read() - start) < us_interval) __WFE();
}


#endif // _UBIRCH_TIMER_H
