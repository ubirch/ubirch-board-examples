/**
 * ubirch#1 SIM800H cell core functionality.
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

#include "sim800h_core.h"
#include "sim800h_parser.h"
#include <drivers/fsl_lpuart.h>
#include <timer.h>

#ifndef BOARD_CELL_PORT
#  error "No PORT found for cell phone chip. please configure ports/pins/clocks!"
#endif

#ifdef NDEBUG
# undef PRINTF
# define PRINTF(...)
#endif

#define GSM_RINGBUFFER_SIZE 32
static uint8_t gsmUartRingBuffer[GSM_RINGBUFFER_SIZE];
static volatile int gsmRxIndex, gsmRxHead;

/**
 * The
 */
void BOARD_CELL_UART_IRQ_HANDLER(void) {
  if ((kLPUART_RxDataRegFullFlag) & LPUART_GetStatusFlags(BOARD_CELL_UART)) {
    uint8_t data = LPUART_ReadByte(BOARD_CELL_UART);

    // it may be necessary to create a critical section here, but
    // right now it didn't hurt us to not disable interrupts

     __disable_irq();
    /* If ring buffer is not full, add data to ring buffer. */
    if (((gsmRxIndex + 1) % GSM_RINGBUFFER_SIZE) != gsmRxHead) {
      gsmUartRingBuffer[gsmRxIndex++] = data;
      gsmRxIndex %= GSM_RINGBUFFER_SIZE;
    }
     __enable_irq();
  }
}

void sim800h_init() {
  const gpio_pin_config_t OUTTRUE = {kGPIO_DigitalOutput, true};
  const gpio_pin_config_t IN = {kGPIO_DigitalInput, false};

  // initialize BOARD_CELL pins
  CLOCK_EnableClock(BOARD_CELL_PORT_CLOCK);
  PORT_SetPinMux(BOARD_CELL_PORT, BOARD_CELL_UART_TX_PIN, BOARD_CELL_UART_TX_ALT);
  PORT_SetPinMux(BOARD_CELL_PORT, BOARD_CELL_UART_RX_PIN, BOARD_CELL_UART_RX_ALT);

  PORT_SetPinMux(BOARD_CELL_PORT, BOARD_CELL_STATUS_PIN, kPORT_MuxAsGpio);
  GPIO_PinInit(BOARD_CELL_GPIO, BOARD_CELL_STATUS_PIN, &IN);

  PORT_SetPinMux(BOARD_CELL_PORT, BOARD_CELL_RESET_PIN, kPORT_MuxAsGpio);
  GPIO_PinInit(BOARD_CELL_GPIO, BOARD_CELL_RESET_PIN, &OUTTRUE);

  PORT_SetPinMux(BOARD_CELL_PORT, BOARD_CELL_PWRKEY_PIN, kPORT_MuxAsGpio);
  GPIO_PinInit(BOARD_CELL_GPIO, BOARD_CELL_PWRKEY_PIN, &OUTTRUE);

  // the ring identifier is optional, only use if a pin and port
#if BOARD_CELL_RI_PIN
  PORT_SetPinMux(BOARD_CELL_PORT, BOARD_CELL_RI_PIN, kPORT_MuxAsGpio);
  GPIO_PinInit(BOARD_CELL_GPIO, BOARD_CELL_RI_PIN, &IN);
#endif

  // configure uart driver connected to the SIM800H
  lpuart_config_t lpuart_config;
  LPUART_GetDefaultConfig(&lpuart_config);
  lpuart_config.baudRate_Bps = 115200;
  lpuart_config.parityMode = kLPUART_ParityDisabled;
  lpuart_config.stopBitCount = kLPUART_OneStopBit;
  LPUART_Init(BOARD_CELL_UART, &lpuart_config, BOARD_CELL_PORT_CLOCK_FREQ);
  LPUART_EnableRx(BOARD_CELL_UART, true);
  LPUART_EnableTx(BOARD_CELL_UART, true);

  LPUART_EnableInterrupts(BOARD_CELL_UART, kLPUART_RxDataRegFullInterruptEnable);
  EnableIRQ(BOARD_CELL_UART_IRQ);
}

void sim800h_enable() {
#if (defined BOARD_CELL_PWR_EN_GPIO) && (defined BOARD_CELL_PWR_EN_PIN)
  // enable power domain and check that VBAT reading is above 2000
  const gpio_pin_config_t OUTFALSE = {kGPIO_DigitalOutput, false};
  // the clock enable for BOARD_CELL_PWR_EN is done in board.c
  GPIO_PinInit(BOARD_CELL_PWR_EN_GPIO, BOARD_CELL_PWR_EN_PIN, &OUTFALSE);
  GPIO_WritePinOutput(BOARD_CELL_PWR_EN_GPIO, BOARD_CELL_PWR_EN_PIN, true);

  // TODO check power for GSM chip
/*
  uint16_t bat;
  while ((bat = VBat_Read()) < 2000) {
    PRINTF("%d\r", bat);
  }
  PRINTF("%d\r\n", bat);
*/
#endif

  // after enabling power, power on the SIM800
  char tmp[10];
  size_t len;

  // we need to identify if the chip is already on by sending AT commands
  // send AT and just ignore the echo and OK to get into a stable state
  // sometimes there is initial noise on the serial line
  sim800h_writeline("AT");
  sim800h_readline(tmp, 9, 100);
  sim800h_readline(tmp, 9, 100);

  // now identify if the chip is actually on, by issue AT and expecting something
  // if we can't read a response, either AT or OK, we need to run the power on sequence
  sim800h_writeline("AT");
  len = sim800h_readline(tmp, 9, 1000);
  if(!len) {
    PRINTF("GSM -OFF !! PWRKEY\r\n");
    // power on the SIM800H
    GPIO_WritePinOutput(BOARD_CELL_GPIO, BOARD_CELL_PWRKEY_PIN, true);
    delay(10); //10ms
    GPIO_WritePinOutput(BOARD_CELL_GPIO, BOARD_CELL_PWRKEY_PIN, false);
    delay(1100); // 1.1s
    GPIO_WritePinOutput(BOARD_CELL_GPIO, BOARD_CELL_PWRKEY_PIN, true);
  } else {
    PRINTF("GSM -ON- !! OK\r\n");
  }

  // wait for the chip to boot and react to commands
  for(int i = 0; i < 5; i++) {
    sim800h_writeline("ATE0");
    // if we can't read anything, try again
    if(!sim800h_readline(tmp, 9, 5000)) continue;
    // if the result is ATE0 instead of OK, expect OK next, else leave it be
    if (!strncmp(tmp, "ATE0", MIN(len, 4)))
      sim800h_readline(tmp, 9, 5000);
    break;
  }
}

void sim800h_disable() {
  // try to power down the SIM800, then switch off power domain
  sim800h_send("AT+CPOWD=1");
  sim800h_expect_urc(14, 1000);

#if ((defined BOARD_CELL_PWR_EN_GPIO) && (defined BOARD_CELL_PWR_EN_PIN))
  GPIO_WritePinOutput(BOARD_CELL_PWR_EN_GPIO, BOARD_CELL_PWR_EN_PIN, false);
#endif
}

int sim800h_read() {
  if ((gsmRxHead % GSM_RINGBUFFER_SIZE) == gsmRxIndex) return -1;
  int c = gsmUartRingBuffer[gsmRxHead++];
  gsmRxHead %= GSM_RINGBUFFER_SIZE;
  return c;
}

size_t sim800h_readline(char *buffer, size_t max, uint32_t timeout) {
  uint32_t us_target = timer_read() + timeout * 1000;
  timer_schedule(us_target);
  size_t idx = 0;
  while (true) {
    if (timer_read() > us_target) break;

    int c = sim800h_read();
    if (c == -1) {
      // nothing in the buffer, allow some sleep
      __WFI();
      continue;
    }

    if (c == '\r') continue;
    if (c == '\n') {
      if (!idx) {
        idx = 0;
        continue;
      }
      break;
    }
    if (max - idx) buffer[idx++] = (char) c;
  }

  buffer[idx] = 0;
  return idx;
}

void sim800h_writeline(const char *buffer) {
  LPUART_WriteBlocking(BOARD_CELL_UART, (const uint8_t *) buffer, strlen(buffer));
  LPUART_WriteBlocking(BOARD_CELL_UART, (const uint8_t *) "\r\n", 2);
}
