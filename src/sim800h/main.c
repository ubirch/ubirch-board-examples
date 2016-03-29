/**
 * Simple SIM800 GSM Modem Console.
 */

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <sim800.h>
#include "ubirch_gsm.h"


void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  bool on = (counter % 50) < 40;  //long on
  LED_Write(on);
}

int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);

  gsm_power_enable();
  gsm_enable();

  LPUART_EnableInterrupts(GSM_UART, kLPUART_RxDataRegFullInterruptEnable);
  EnableIRQ(GSM_UART_IRQ);

  uint8_t buffer[128], idx = 0, newline[2] = {'\r', '\n' };

  PRINTF("] ");
  while (true) {
    uint8_t ch = GETCHAR();
    if (ch == '\r') {
      PUTCHAR('\r');
      PUTCHAR('\n');
      LPUART_WriteBlocking(GSM_UART, buffer, idx);
      LPUART_WriteBlocking(GSM_UART, newline, 2);
      idx = 0;
      PRINTF("] ");
    } else {
      PUTCHAR(ch);
      buffer[idx++] = ch;
    }
  }
}

void GSM_UART_IRQ_HANDLER(void) {
  if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(GSM_UART)) {
    PUTCHAR(LPUART_ReadByte(GSM_UART));
  }
}

