#include "ubirch_gsm.h"

void gsm_power_enable() {
  const gpio_pin_config_t OUTFALSE = {kGPIO_DigitalOutput, false};
  // the clock enable for GSM_PWR_EN is done in board.c
  GPIO_PinInit(GSM_PWR_EN_GPIO, GSM_PWR_EN_PIN, &OUTFALSE);
  GPIO_WritePinOutput(GSM_PWR_EN_GPIO, GSM_PWR_EN_PIN, true);

  uint16_t bat;
  while((bat = VBat_Read()) < 2000) {
    PRINTF("%d\r", bat);
  }
  PRINTF("%d\r\n", bat);
}

void gsm_power_disable() {
  GPIO_WritePinOutput(GSM_PWR_EN_GPIO, GSM_PWR_EN_PIN, false);
}


void gsm_enable() {
  const gpio_pin_config_t OUTTRUE = {kGPIO_DigitalOutput, true};
  const gpio_pin_config_t IN = {kGPIO_DigitalInput, false};

  // initialize GSM pins
  CLOCK_EnableClock(GSM_PORT_CLOCK);
  PORT_SetPinMux(GSM_PORT, GSM_UART_TX_PIN, GSM_UART_TX_ALT);
  PORT_SetPinMux(GSM_PORT, GSM_UART_RX_PIN, GSM_UART_RX_ALT);

  PORT_SetPinMux(GSM_PORT, GSM_STATUS_PIN, GSM_STATUS_ALT);
  GPIO_PinInit(GSM_GPIO, GSM_STATUS_PIN, &IN);

  PORT_SetPinMux(GSM_PORT, GSM_RESET_PIN, GSM_RESET_ALT);
  GPIO_PinInit(GSM_GPIO, GSM_RESET_PIN, &OUTTRUE);

  PORT_SetPinMux(GSM_PORT, GSM_PWRKEY_PIN, GSM_PWRKEY_ALT);
  GPIO_PinInit(GSM_GPIO, GSM_PWRKEY_PIN, &OUTTRUE);

  PORT_SetPinMux(GSM_PORT, GSM_RI_PIN, GSM_RI_ALT);
  GPIO_PinInit(GSM_GPIO, GSM_RI_PIN, &IN);

  lpuart_config_t lpuart_config;
  LPUART_GetDefaultConfig(&lpuart_config);
  lpuart_config.baudRate_Bps = 115200;
  lpuart_config.parityMode = kLPUART_ParityDisabled;
  lpuart_config.stopBitCount = kLPUART_OneStopBit;
  LPUART_Init(GSM_UART, &lpuart_config, LPUART_BASE_CLOCK);
  LPUART_EnableRx(GSM_UART, true);
  LPUART_EnableTx(GSM_UART, true);

  GPIO_WritePinOutput(GSM_GPIO, GSM_PWRKEY_PIN, true);
  BusyWait100us(100); //10ms
  GPIO_WritePinOutput(GSM_GPIO, GSM_PWRKEY_PIN, false);
  BusyWait100us(11000); // 1.1s
  GPIO_WritePinOutput(GSM_GPIO, GSM_PWRKEY_PIN, true);
}
