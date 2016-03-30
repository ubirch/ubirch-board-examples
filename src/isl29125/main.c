#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <extpin.h>
#include <drivers/fsl_lpuart.h>
#include "isl29125.h"

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  LED_Write((counter % 100) < 10);
}


int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);
  PRINTF("\r\n-- ISL29125 test\r\n");

  CLOCK_EnableClock(kCLOCK_PortB);
  PORT_SetPinMux(PORTB, 10, kPORT_MuxAlt4);
  PORT_SetPinMux(PORTB, 11, kPORT_MuxAlt4);

  // configure I2C
  i2c_master_config_t i2c_config;
  I2C_MasterGetDefaultConfig(&i2c_config);
  i2c_config.baudRate_Bps = 100000U;
  I2C_MasterInit(I2C2, &i2c_config, CLOCK_GetFreq(kCLOCK_BusClk));

  if (!isl_reset()) {
    PRINTF("could not initialize ISL29125 RGB sensor\r\n");
  }

  // set sampling mode, ir filter and interrupt mode
  isl_set(ISL_R_COLOR_MODE, ISL_MODE_RGB | ISL_MODE_375LUX | ISL_MODE_16BIT);
  isl_set(ISL_R_FILTERING, ISL_FILTER_IR_MAX);
  isl_set(ISL_R_INTERRUPT, ISL_INT_ON_THRSLD);

  uint8_t color_mode = isl_get(ISL_R_COLOR_MODE);
  uint8_t filter_mode = isl_get(ISL_R_FILTERING);
  uint8_t intr = isl_get(ISL_R_INTERRUPT);
  PRINTF("read mode      : 0x%02x (%08b)\r\n", color_mode, color_mode);
  PRINTF("read filter    : 0x%02x (%08b)\r\n", filter_mode, filter_mode);
  PRINTF("read interrupts: 0x%02x (%08b)\r\n", intr, intr);

  PRINTF("reading RGB values from sensor\r\n");
  PRINTF("'%%' indicates the chip is still in a conversion cyle, so we wait\r\n");
  while (true) {
    // wait for the conversion cycle to be done, this just indicates there is a cycle
    // in progress. the actual r,g,b values are always available from the last cycle
    while (!(isl_get(ISL_R_STATUS) & ISL_STATUS_ADC_DONE)) PUTCHAR('%');
    PRINTF("\r\n");

    // read the full 36 or 48 bit color
    printf("48bit: ");
    rgb48_t rgb48;
    isl_read_rgb48(&rgb48);
    printf("0x%04x%04x%04x rgb48(%u,%u,%u)\r\n", rgb48.red, rgb48.green, rgb48.blue, rgb48.red, rgb48.green, rgb48.blue);

    printf("24bit: ");
    rgb24_t rgb24;
    isl_read_rgb24(&rgb24);
    printf("0x%02x%02x%02x rgb24(%u,%u,%u)\r\n", rgb24.red, rgb24.green, rgb24.blue, rgb24.red, rgb24.green, rgb24.blue);

    BusyWait100us(50000);
  }

  I2C_MasterStop(I2C2);
  I2C_MasterDeinit(I2C2);

  return 0;

}
