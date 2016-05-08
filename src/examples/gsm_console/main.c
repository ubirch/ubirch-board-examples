/**
 * Simple SIM800 GSM Modem Console.
 */

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <sim800h_core.h>
#include <sim800h_ops.h>
#include "config.h"

static char buffer[128];
static volatile uint16_t idx = 0;

void SysTick_Handler() {
  static uint8_t counter = 0;
  // we misuse the systick handler to read from the sim800h ringbuffer
  int c = sim800h_read();
  if(c != -1 && idx < 127 && c != '\r') {
    if(c == '\n') {
      buffer[idx] = '\0';
      PRINTF("%s\r\n", buffer);
      idx = 0;
    } else buffer[idx++] = (char) c;
  }

  static bool on = true;
  if((counter++ % 100) == 0) on = !on;
  BOARD_LED0(on);
}

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  sim800h_init();
  sim800h_enable();
  sim800h_register(60000);
  sim800h_gprs_attach(CELL_APN, CELL_USER, CELL_PWD, 60000);

  SysTick_Config(BOARD_SYSTICK_100MS);

  PRINTF("GSM console ready.\r\n");
  uint8_t buffer[128], idx = 0;
  while (true) {
    int ch = GETCHAR();
    if (ch == '\r' || ch == '\n') {
      PUTCHAR('\r');
      PUTCHAR('\n');
      buffer[idx] = '\0';
      if(!strncasecmp((const char *) buffer, "quit",  MIN(4, idx))) break;
      sim800h_writeline((const char *) buffer);
      idx = 0;
    } else {
      PUTCHAR(ch);
      buffer[idx++] = (uint8_t) ch;
    }
  }

  sim800h_disable();

  return 0;
}
