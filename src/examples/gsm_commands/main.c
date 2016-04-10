/**
 * Working on AT command parsing.
 */

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <sim800h.h>

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  bool on = (counter % 50) < 40;  //long on
  LED_Write(on);
}

// this useful list found here: https://github.com/cloudyourcar/attentive
const char *SIM800H_URC[] = {
  "+CIPRXGET: 1,",  /*! incoming socket data notification */
  "+FTPGET: 1,",    /*! FTP state change notification */
  "+PDP: DEACT",    /*! PDP disconnected */
  "+SAPBR 1: DEACT",/*! PDP disconnected (for SAPBR apps) */
  "*PSNWID:",       /*! AT+CLTS network name */
  "*PSUTTZ:",       /*! AT+CLTS time */
  "+CTZV:",         /*! AT+CLTS timezone */
  "DST:",           /*! AT+CLTS dst information */
  "+CIEV:",         /*! AT+CLTS undocumented indicator */
  "RDY",            /*! Device ready. */
  "+CFUN: 1",       /*! Device has entered full functional mode */
  "+CPIN: READY",
  "Call Ready",
  "SMS Ready",
  "NORMAL POWER DOWN",
  "UNDER-VOLTAGE POWER DOWN",
  "UNDER-VOLTAGE WARNNING",
  "OVER-VOLTAGE POWER DOWN",
  "OVER-VOLTAGE WARNNING",
  NULL
};

int is_urc(char *line) {
  size_t len = strlen(line);
  for (int i = 0; SIM800H_URC[i] != NULL; i++) {
    const char *urc = SIM800H_URC[i];
    size_t urc_len = strlen(SIM800H_URC[i]);
    if (len >= urc_len && !strncmp(urc, line, urc_len)) {
      line[len] = 0;
      PRINTF("GSM INFO !! [%02d] %s\r\n", i, line);
      return i;
    }
  }
  return -1;
}

void gsm_cmd(const char *cmd) {
  PRINTF("GSM (%02d) <- %s\r\n", strlen(cmd), cmd);
  LPUART_WriteBlocking(GSM_UART, (const uint8_t *) cmd, strlen(cmd));
  LPUART_WriteBlocking(GSM_UART, (const uint8_t *) "\r\n", 2);
}

void gsm_expect_urc(int n) {
  char response[128] = {0};
  do {
    sim800h_readline(response, 127);
    PRINTF("GSM .... ?? %s\r\n", response);
  } while (is_urc(response) != n);
}

bool gsm_expect(const char *expected) {
  char response[255] = {0};
  size_t len, expected_len = strlen(expected);
  while (true) {
    len = sim800h_readline(response, 127);
    if (is_urc(response) >= 0) continue;
    PRINTF("GSM (%02d) -> %s\r\n", len, response);
    return strncmp(expected, (const char *) response, MIN(len, expected_len)) == 0;
  }
}

int main(void) {
  BOARD_Init();
  SysTick_Config(RUN_SYSTICK_10MS);

  // prepare GSM module
  sim800h_enable();

  // power on GSM module
  sim800h_power_enable();

  gsm_expect_urc(9);
  // disable echo, we need to check if our command is echoed back, then
  // check for OK - if echo is off we just get a failed ATE0
  gsm_cmd("ATE0");
  if (gsm_expect("ATE0"))
    gsm_expect("OK");

  PRINTF("----- ECHO OFF\r\n");

  gsm_cmd("ATV1");
  gsm_expect("OK");

  gsm_cmd("ATI");
  gsm_expect("SIM");
  gsm_expect("OK");

  PRINTF("----- SIM INFO RECEIVED\r\n");

  bool registered;
  do {
    BusyWait100us(20000);
    gsm_cmd("AT+CREG?");
    // this looks strange because we need to ensure both gsm_expect() calls are made,
    // so expect-1 && expect-2 does not work because of expression evaluation
    registered = gsm_expect("+CREG: 0,5");
    registered = gsm_expect("OK") && registered;
  } while (!registered);

  PRINTF("----- GSM REGISTERED WITH NETWORK\r\n");

  gsm_cmd("AT+CIPSHUT");
  gsm_expect("SHUT OK");

  gsm_cmd("AT+CIPMUX=1");
  gsm_expect("OK");

  gsm_cmd("AT+CIPRXGET=1");
  gsm_expect("OK");

  do {
    BusyWait100us(20000);
    gsm_cmd("AT+CGATT=1");
  } while (!gsm_expect("OK"));

  PRINTF("----- INIT DONE\r\n");

  char buffer[128];
  while (true) {
    buffer[0] = 0;
    sim800h_readline(buffer, 127);
    switch (is_urc(buffer)) {
      case 13: {
        gsm_cmd("AT+CPOWD=1");
        gsm_expect_urc(14);
        break;
      }
      case 14: {
        sim800h_power_disable();
        break;
      }
      default: {
        PRINTF(">> %s\r\n", buffer);
      }
    }
  }
}

