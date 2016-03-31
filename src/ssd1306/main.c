#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <extpin.h>
#include <drivers/fsl_lpuart.h>
#include "ssd1306.h"
#include "../libs/i2c/i2c_core.h"
#include "font5x5.h"

void invert_char(uint8_t *buffer, uint8_t r, uint8_t c);

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  LED_Write((counter % 100) < 10);
}


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

  uint8_t buffer[64 * 6];
  memset(buffer, 0x00, 64 * 6);

  for (uint8_t c = 0; c < 26; c++) {
    memcpy(buffer + c * 8+1, font5x5_abc + c * 5, 5);
  }

  while (true) {
    for (int r = 0; r < 6; r++) {
      for (int c = 0; c < 8; c++) {
        // invert a character spot, like a cursor
        invert_char(buffer, r, c);

        // send to device
        oled_data(OLED_DEVICE_ADDRESS, buffer, 64 * 6);
        BusyWait100us(1000);
      }
    }
  }

  PRINTF("DONE\r\n");
  return 0;
}

void invert_char(uint8_t *buffer, uint8_t r, uint8_t c) {
  // keep last cursor position
  static uint8_t cr = 0xff, cc;
  if(cr != 0xff) for (int b = 0; b < 7; b++) buffer[cr * 64 + cc * 8 + b] = ~buffer[cr * 64 + cc * 8 + b];
  cr = r;
  cc = c;
  for (int b = 0; b < 7; b++) buffer[cr * 64 + cc * 8 + b] = ~buffer[cr * 64 + cc * 8 + b];
}

/*
 * byte[] display = new byte[504];

// storing these means they don't need to be calculated every time:
byte[] base2 = { 1, 2, 4, 8, 16, 32, 64, 128 };

// Determine if the pixel is ON (true) or OFF (false)
public bool PixelState(Pixel px)
{
    return (display[GetColumn(px)] & base2[GetPxNum(px)]) > 0;
}

// Find the number of the pixel in its column of 8
private int GetPxNum(Pixel px)
{
    return px.y % 8;
}

// Find the index of the byte containing the bit representing the state of a pixel
private int GetColumn(Pixel px)
{
    return (px.y / 8 * 84) + px.x;
}

// Set a pixel's state
public void SetPixel(Pixel px, bool state)
{
    int col = GetColumn(px);
    int num = GetPxNum(px);

    if (state && !PixelState(px))
        display[col] += base2[num];
    else if (!state && PixelState(px))
        display[col] -= base2[num];
}

// Represents one (x,y) point
public struct Pixel
{
    public int x, y;

    public Pixel(int x, int y)
    {
        this.x = x;
        this.y = y;
    }
}
 */
