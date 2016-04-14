/**
 * Simple SIM800 GSM Modem Console.
 */

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <sim800h.h>

static char buffer[128];
static volatile uint8_t idx = 0;

void SysTick_Handler() {
  // we misuse the systick handler to read from the sim800h ringbuffer
  int c = sim800_read();
  if(c != -1 && idx < 127 && c != '\r') {
    if(c == '\n') {
      buffer[idx] = '\0';
      PRINTF("%s\r\n", buffer);
      idx = 0;
    } else buffer[idx++] = (char) c;
  }

  // still do the blinking
  static uint32_t counter = 0;
  counter++;
  bool on = (counter % 50) < 40;  //long on
  LED_Write(on);
}

int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);

  sim800h_enable();
  sim800h_power_enable();

  uint8_t buffer[128], idx = 0;
  while (true) {
    int ch = GETCHAR();
    if (ch == '\r' || ch == '\n') {
      PUTCHAR('\r');
      PUTCHAR('\n');
      buffer[idx] = '\0';
      sim800h_writeline((const char *) buffer);
      idx = 0;
    } else {
      PUTCHAR(ch);
      buffer[idx++] = (uint8_t) ch;
    }
  }
}
