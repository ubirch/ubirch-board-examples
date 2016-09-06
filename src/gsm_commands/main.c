/**
 * Working on AT command parsing.
 */

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <ubirch/modem.h>
#include <ubirch/timer.h>

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  bool on = (counter % 50) < 40;  //long on
  BOARD_LED0(on);
}

#define TIMEOUT 5000

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(BOARD_SYSTICK_100MS);

  modem_init();
  modem_enable();

  if (!modem_expect_urc(9, TIMEOUT))
    PRINTF("RDY expected, not found\r\n");

  // disable echo, we need to check if our command is echoed back, then
  // check for OK - if echo is off we just get a failed ATE0
  modem_send("ATE0");
  if (modem_expect("ATE0", TIMEOUT))
    modem_expect_OK(TIMEOUT);

  PRINTF("----- ECHO OFF\r\n");

  modem_send("ATV1");
  modem_expect_OK(TIMEOUT);

  modem_send("ATI");
  modem_expect("SIM", TIMEOUT);
  modem_expect_OK(TIMEOUT);

  PRINTF("----- SIM INFO RECEIVED\r\n");

  bool registered;
  do {
    delay(2000);
    modem_send("AT+CREG?");
    // this looks strange because we need to ensure both gsm_expect() calls are made,
    // so expect-1 && expect-2 does not work because of expression evaluation
    registered = modem_expect("+CREG: 0,5", TIMEOUT);
    registered = modem_expect_OK(TIMEOUT) && registered;
  } while (!registered);

  PRINTF("----- GSM REGISTERED WITH NETWORK\r\n");

  modem_send("AT+CIPSHUT");
  modem_expect("SHUT OK", TIMEOUT);

  modem_send("AT+CIPMUX=1");
  modem_expect_OK(TIMEOUT);

  modem_send("AT+CIPRXGET=1");
  modem_expect_OK(TIMEOUT);

  do {
    delay(2000);
    modem_send("AT+CGATT=1");
  } while (!modem_expect_OK(TIMEOUT));

  PRINTF("----- INIT DONE\r\n");

  char buffer[128];
  while (true) {
    buffer[0] = 0;
    if (modem_readline(buffer, 127, 500)) {
      switch (modem_check_urc(buffer)) {
        case -1: {
          PRINTF(">> %s\r\n", buffer);
          break;
        }
        case 13: {
          modem_send("AT+CPOWD=1");
          break;
        }
        case 14: {
          modem_disable();
          break;
        }
        default:
          break;
      }
    }
  }
}

