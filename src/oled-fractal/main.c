#include <stdint.h>
#include <stdio.h>
#include <utilities/fsl_debug_console.h>
#include <board.h>
#include <ubirch/timer.h>
#include <ubirch/i2c.h>
#include <ubirch/i2c/ssd1306.h>
#include "font5x5.h"

static i2c_config_t i2c_config = {
  .port = BOARD_I2C_PORT,
  .mux = BOARD_I2C_ALT,
  .port_clock = BOARD_I2C_PORT_CLOCK,
  .i2c = BOARD_I2C,
  .i2c_clock = BOARD_I2C_CLOCK,
  .SCL = BOARD_I2C_SCL_PIN,
  .SDA = BOARD_I2C_SDA_PIN,
  .baud = I2C_FAST_MODE
};


void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  BOARD_LED0((counter % 100) < 10);
}

uint8_t buffer[64 * 6];

void putpixel(int column, int row, int color) {
  if (color) {
    *(buffer + (row / 8) * 64 + column) |= (uint8_t) (1 << (row % 8));
  } else {
    *(buffer + (row / 8) * 64 + column) &= (uint8_t) ~(1 << (row % 8));
  }
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

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(BOARD_SYSTICK_100MS);

  PRINTF("\r\n-- SSD1306 test\r\n");

  // initialize i2c
  i2c_init(&i2c_config);

  // enable reset pin clock, mux as GPIO and reset oled display
  CLOCK_EnableClock(kCLOCK_PortB);
  PORT_SetPinMux(PORTB, 9, kPORT_MuxAsGpio);
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

  while(1) {
    int width = 48, height = 64;
    int max = 50;
    int black = 0, white = 1;


    for (int i = 1; i < max; i++) {
      for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
          double c_re = (col - width / 2.0) * 4.0 / width;
          double c_im = (row - height / 2.0) * 4.0 / height;
          double x = 0, y = 0;
          int iteration = 0;
          while (x * x + y * y <= 4 && iteration < i) {
            double x_new = x * x - y * y + c_re;
            y = 2 * x * y + c_im;
            x = x_new;
            iteration++;
          }
          if (iteration < i) putpixel(row, col, black);
          else putpixel(row, col, white);
        }
      }
      ssd1306_data(OLED_DEVICE_ADDRESS, buffer, 64 * 6);
      delay(100);
    }

  }
  PRINTF("DONE\r\n");
  return 0;
}

