#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include "ssd1306.h"
#include <i2c.h>
#include <utilities/fsl_debug_console.h>
#include <board.h>
#include "font5x5.h"

void oled_invert(int row, int column);

void oled_putc(int row, int column, char c);

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  BOARD_LED0((counter % 100) < 10);
}

uint8_t buffer[64 * 6];

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(SystemCoreClock / 100U);

  PRINTF("\r\n-- SSD1306 test\r\n");

  // initialize i2c
  i2c_init(I2C_FAST_MODE);

  // enable reset pin clock, mux correcly and reset oled display
  CLOCK_EnableClock(kCLOCK_PortB);
  PORT_SetPinMux(PORTB, 9, kPORT_MuxAlt3);

  ssd1306_reset(GPIOB, 9);

  delay(1);

  const status_t status = i2c_ping(OLED_DEVICE_ADDRESS);
  if (status == kStatus_Success) {
    PRINTF("OLED device found!\r\n");
  } else {
#ifndef NDEBUG
    i2c_error("OLED device not found", status);
#endif
    PRINTF("OLED device not found.\r\n");
  };

  ssd1306_clear(OLED_DEVICE_ADDRESS);

  ssd1306_cmd(OLED_DEVICE_ADDRESS, OLED_DISPLAY_ON);

  // set the display constraints
  ssd1306_cmd(OLED_DEVICE_ADDRESS, OLED_PAGE_ADDRESS);
  ssd1306_cmd(OLED_DEVICE_ADDRESS, 0x00);
  ssd1306_cmd(OLED_DEVICE_ADDRESS, 0x05);

  ssd1306_cmd(OLED_DEVICE_ADDRESS, OLED_COLUMN_ADDRESS);
  ssd1306_cmd(OLED_DEVICE_ADDRESS, 0x20);
  ssd1306_cmd(OLED_DEVICE_ADDRESS, 0x20 + 63);

  memset(buffer, 0x00, 64 * 6);
  ssd1306_data(OLED_DEVICE_ADDRESS, buffer, 64 * 6);

  int row = 0, column = 0;
  oled_invert(row, column);

  // write all characters read from the debug console
  // to the display and move cursor forward, wraps at the
  // display boundary (8x6)
  // stops when escape is pressed
  char c;
  do {
    c = (uint8_t) toupper(GETCHAR());
    oled_invert(row, column);
    switch (c) {
      case '\r':
      case '\n':
        column = -1;
            row++;
            break;
      case ' ':
        break;
      default:
        oled_putc(row, column, c);
            break;
    }
    if (++column > 7) {
      column = 0;
      row++;
    }
    if (row > 5) row = 0;
    oled_invert(row, column);
  } while (c != '\e');

  PRINTF("DONE\r\n");
  return 0;
}

void oled_putc(int row, int column, char c) {
  if (c >= 'A' && c <= 'Z') {
    memcpy(buffer + row * 64 + column * 8 + 1, font5x5_abc + (c - 'A') * 5, 5);
  } else {
    memcpy(buffer + row * 64 + column * 8 + 1, &font5x5_extra[20 * 5], 5);
  }
}

void oled_invert(int row, int column) {
  for (int b = 0; b < 7; b++) buffer[row * 64 + column * 8 + b] = ~buffer[row * 64 + column * 8 + b];
  ssd1306_data(OLED_DEVICE_ADDRESS, buffer, 64 * 6);
}
