/*! Blink - simplistic app. Blinks LED, reverses blink pattern on button press.
 * Written by Matthias Krauss
 * Copyright 2016 Press Every Key UG
 */


#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <extpin.h>
#include <drivers/fsl_lpuart.h>

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  LED_Write((counter % 100) < 10);
}

int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);

  // initialize I2C
  i2c_master_config_t i2c_config;
  I2C_MasterGetDefaultConfig(&i2c_config);
  i2c_config.baudRate_Bps = 400000U;
  I2C_MasterInit(I2C2, &i2c_config, CLOCK_GetFreq(kCLOCK_BusClk));

  PRINTF("WELCOME!\r\n");
  while(true) {
    uint8_t ch = GETCHAR();
    if(ch == '\r') PUTCHAR('\n');
    PUTCHAR(ch);
  }
}
