#include <stdint.h>
#include <board.h>
#include <stdio.h>
#include <extpin.h>
#include <ctype.h>
#include "ssd1306.h"
#include "../libs/i2c/i2c_core.h"
#include "font5x5.h"

void oled_invert(int row, int column);

void oled_puts(int row, int column, char *s) ;

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  LED_Write((counter % 100) < 10);
}

uint8_t buffer[64 * 6];

int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);
  PRINTF("\r\n-- SSD1306 test\r\n");

  // reset oled display
  oled_reset(EXTPIN_3);

  BusyWait100us(1000);

  // initialize i2c
  i2c_init(I2C_FAST_MODE);
  const status_t status = i2c_ping(OLED_DEVICE_ADDRESS);
  if (status == kStatus_Success) {
    PRINTF("OLED device found!\r\n");
  } else {
    i2c_error("OLED device not found", status);
  };


  // software configuration according to specs
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_DISPLAY_OFF);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_CLOCK_DIV_FREQ);
  oled_cmd(OLED_DEVICE_ADDRESS, 0b100000); // 0x80: 1000 (freq) 00 (divider)
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_MULTIPLEX_RATIO);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x2F);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_DISPLAY_OFFSET);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x00);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_START_LINE | 0x00);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_CHARGE_PUMP);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x14);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_SCAN_REVERSE);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_SEGMENT_REMAP1);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_COM_PIN_CONFIG);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x12);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_CONTRAST);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x10);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_PRECHARGE_PERIOD);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x22);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_VCOM_DESELECT);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x00);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_DISPLAY_RESUME);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_DISPLAY_NORMAL);

  oled_cmd(OLED_DEVICE_ADDRESS, OLED_ADDRESSING_MODE);
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_ADDR_MODE_HORIZ);

  oled_clear(OLED_DEVICE_ADDRESS);

  oled_cmd(OLED_DEVICE_ADDRESS, OLED_DISPLAY_ON);

  // set the display constraints
  oled_cmd(OLED_DEVICE_ADDRESS, OLED_PAGE_ADDRESS);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x00);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x05);

  oled_cmd(OLED_DEVICE_ADDRESS, OLED_COLUMN_ADDRESS);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x20);
  oled_cmd(OLED_DEVICE_ADDRESS, 0x20 + 63);

  memset(buffer, 0x00, 64 * 6);
  oled_data(OLED_DEVICE_ADDRESS, buffer, 64*6);

  int row = 0, column = 0;
  oled_invert(row, column);

  char c[8];
  c[1] = 0;
  do {
    c[0] = (uint8_t) toupper(GETCHAR());
    oled_invert(row, column);
    if(c[0] != ' ') oled_puts(row, column, c);
    if(column++ > 7) {
      column = 0;
      row++;
    }
    if(row > 5) row = 0;
    oled_invert(row, column);
  } while(c[0] != '\r');

  PRINTF("DONE\r\n");
  return 0;
}

void oled_puts(int row, int column, char *s) {
  while (*s) {
    char c = *s++;
    if (c >= 'A' && c <= 'Z') {
      memcpy(buffer + row * 64 + column * 8 + 1, font5x5_abc + (c - 'A') * 5, 5);
    } else {
      memcpy(buffer + row * 64 + column * 8 + 1, &font5x5_extra[20 * 5], 5);
    }
  }
}

void oled_invert(int row, int column) {
  for (int b = 0; b < 7; b++) buffer[row * 64 + column * 8 + b] = ~buffer[row * 64 + column * 8 + b];
  oled_data(OLED_DEVICE_ADDRESS, buffer, 64*6);
}
