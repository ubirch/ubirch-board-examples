#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <utilities/fsl_debug_console.h>
#include <board.h>
#include <ubirch/i2c.h>
#include <ubirch/timer.h>
#include <ubirch/i2c/ssd1306.h>
#include "ugui/ugui.h"

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  BOARD_LED0((counter % 100) < 10);
}

uint8_t buffer[64 * 6];

void putpixel(int column, int row, int color) {
  if (color != C_BLACK) {
    *(buffer + (row / 8) * 64 + column) |= (uint8_t) (1 << (row % 8));
  } else {
    *(buffer + (row / 8) * 64 + column) &= (uint8_t) ~(1 << (row % 8));
  }
}

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(BOARD_SYSTICK_100MS);

  PRINTF("\r\n-- SSD1306 test\r\n");

  // initialize i2c
  i2c_init(&i2c_config_default);

  // enable reset pin clock, mux as GPIO and reset oled display
  CLOCK_EnableClock(kCLOCK_PortB);
  PORT_SetPinMux(PORTB, 2, kPORT_MuxAsGpio);
  ssd1306_reset(GPIOB, 2);

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

  UG_GUI gui;
  UG_Init(&gui, (void (*)(UG_S16, UG_S16, UG_COLOR)) putpixel, 64, 6 * 8);
  UG_FontSelect(&FONT_6X8);
  UG_ConsoleSetArea(0, 0, 63, 6 * 8 - 1);
  UG_ConsoleSetForecolor(C_WHITE);
  UG_ConsoleSetBackcolor(C_BLACK);

  UG_ConsolePutString("Hello World!\n");
  ssd1306_data(OLED_DEVICE_ADDRESS, buffer, 64 * 6);

  // write all characters read from the debug console
  // to the display and move cursor forward, wraps at the
  // display boundary (8x6)
  // stops when escape is pressed
  char c[2] = {0, 0};
  do {
    c[0] = (uint8_t) GETCHAR();
    switch (c[0]) {
      case '\n':
      case '\r': {
        c[0] = '\n';
        break;
      }
      default: {
        UG_ConsolePutString(c);
        ssd1306_data(OLED_DEVICE_ADDRESS, buffer, 64 * 6);
        break;
      }
    }
  } while (c[0] != '\e');

  PRINTF("DONE\r\n");
  return 0;
}

