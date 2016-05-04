/**
 * Working on AT command parsing.
 */

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <sim800h.h>
#include <sim800h_parser.h>

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  bool on = (counter % 50) < 40;  //long on
  BOARD_LED0(on);
}


int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(BOARD_SYSTICK_100MS);

  // prepare GSM module
  sim800h_enable();

  // power on GSM module
  sim800h_power_enable();

  sim800h_expect_urc(9);
  // disable echo, we need to check if our command is echoed back, then
  // check for OK - if echo is off we just get a failed ATE0
  sim800h_send("ATE0");
  if (sim800h_expect("ATE0"))
    sim800h_expect("OK");

  PRINTF("----- ECHO OFF\r\n");

  sim800h_send("ATV1");
  sim800h_expect("OK");

  sim800h_send("ATI");
  sim800h_expect("SIM");
  sim800h_expect("OK");

  PRINTF("----- SIM INFO RECEIVED\r\n");

  bool registered;
  do {
    delay(2000);
    sim800h_send("AT+CREG?");
    // this looks strange because we need to ensure both gsm_expect() calls are made,
    // so expect-1 && expect-2 does not work because of expression evaluation
    registered = sim800h_expect("+CREG: 0,5");
    registered = sim800h_expect("OK") && registered;
  } while (!registered);

  PRINTF("----- GSM REGISTERED WITH NETWORK\r\n");

  sim800h_send("AT+CIPSHUT");
  sim800h_expect("SHUT OK");

  sim800h_send("AT+CIPMUX=1");
  sim800h_expect("OK");

  sim800h_send("AT+CIPRXGET=1");
  sim800h_expect("OK");

  do {
    delay(2000);
    sim800h_send("AT+CGATT=1");
  } while (!sim800h_expect("OK"));

  PRINTF("----- INIT DONE\r\n");

  char buffer[128];
  while (true) {
    buffer[0] = 0;
    sim800h_readline(buffer, 127);
    switch (check_urc(buffer)) {
      case -1: {
        PRINTF(">> %s\r\n", buffer);
        break;
      }
      case 13: {
        sim800h_send("AT+CPOWD=1");
        break;
      }
      case 14: {
        sim800h_power_disable();
        break;
      }
      default: break;
    }
  }
}

