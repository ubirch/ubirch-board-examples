/**
 * Lights Sensor Code.
 *
 * Sample RGB data and send it to the backend, receive configuration
 * settings in the response.
 *
 * @author Matthias L. Jugel
 * @date 2016-04-06
 *
 * Copyright 2016 ubirch GmbH (https://ubirch.com)
 *
 * == LICENSE ==
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <board.h>
#include <ubirch/isl29125/isl29125.h>
#include <ubirch/i2c/i2c.h>
#include <ubirch/timer/timer.h>
#include <ubirch/sim800h/sim800h.h>
#include <ubirch/rtc/rtc.h>
#include <wolfssl/wolfcrypt/sha512.h>
#include <ubirch/crypto/crypto.h>
#include <wolfssl/wolfcrypt/ed25519.h>
#include <ubirch/dbgutil/dbgutil.h>
#include "config.h"

// DEMO key
const unsigned char device_ecc_key[] = {
  0xec, 0x87, 0x3b, 0x71, 0x6f, 0x5d, 0xb1, 0x37, 0x3a, 0x4a, 0x6b, 0x5b,
  0x60, 0x9f, 0x7c, 0xc7, 0xf6, 0x50, 0x16, 0x45, 0x05, 0x68, 0xee, 0x17,
  0x4e, 0xc5, 0xaf, 0xb4, 0xa6, 0x9e, 0xcd, 0x6c, 0x23, 0x9a, 0xf3, 0x94,
  0xc2, 0x62, 0x35, 0x28, 0x80, 0xcd, 0x7f, 0x6a, 0x19, 0x46, 0xc4, 0x7d,
  0xd4, 0x7c, 0xf0, 0x7a, 0xdc, 0x80, 0x46, 0x62, 0x19, 0x0d, 0x94, 0x46,
  0x23, 0x56, 0x17, 0x91
};
const unsigned int device_ecc_key_len = 64;


#define TIMEOUT 5000

// default wakup interval in seconds
#define DEFAULT_INTERVAL 5*60

// protocol version check
#define PROTOCOL_VERSION_MIN "0.0"
// json keys
#define P_SIGNATURE "s"
#define P_VERSION "v"
#define P_PAYLOAD "p"
#define P_SENSITIVITY "s"
#define P_IR_FILTER "ir"
#define P_INTERVAL "i"

// error flags
#define E_SENSOR_FAILED 0b00000001
#define E_PROTOCOL_FAIL 0b00000010
#define E_SIG_VRFY_FAIL 0b00000100
#define E_JSON_FAILED   0b00001000
#define E_NO_MEMORY     0b10000000
#define E_NO_CONNECTION 0b01000000

#define ISL_375LUX_MAX 65000
#define ISL_10KLUX_MIN 3000

// i2c configuration, based on the board used
static const i2c_config_t i2c_config = {
  .i2c = BOARD_I2C,
  .port = BOARD_I2C_PORT,
  .mux = BOARD_I2C_ALT,
  .port_clock = BOARD_I2C_PORT_CLOCK,
  .SCL = BOARD_I2C_SCL_PIN,
  .SDA = BOARD_I2C_SDA_PIN,
  .baud = I2C_FULL_SPEED
};

// crypto key of the board
uc_ed25519_key uc_key;

// this counts up as long as we don't have a reset
//static uint16_t loop_counter = 1;
static uint8_t error_flag = 0x00;
static int loop_counter = 0;

// internal sensor state
//static uint16_t interval = DEFAULT_INTERVAL;
static uint8_t sensitivity = ISL_MODE_375LUX;
static uint8_t infrared_filter = ISL_FILTER_IR_MAX;


void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  BOARD_LED0((counter % 100) < 10);
}

/*!
 * Sample light data via the external RGB sensor.
 *
 * @param red part - passed by reference
 * @param green part - passed by reference
 * @param blue part - passed by reference
 */
bool sample_rgb(rgb48_t *color, uint8_t color_mode) {
  if (!isl_reset()) {
    PRINTF("ISL29125 reset failed\r\n");
    error_flag |= E_SENSOR_FAILED;
    return false;
  }
  // we need to set the filter first, then as sampling starts with the 0x01 (COLOR_MODE) register
  if (!isl_set(ISL_R_FILTERING, infrared_filter)) {
    PRINTF("ISL29125 set infrared filtering failed\r\n");
    error_flag |= E_SENSOR_FAILED;
    return false;
  }
  // set sensitivity and color mode and start sampling
  if (!isl_set(ISL_R_COLOR_MODE, (uint8_t) (color_mode | ISL_MODE_16BIT | ISL_MODE_RGB))) {
    PRINTF("ISL29125 ir config failed\r\n");
    error_flag |= E_SENSOR_FAILED;
    return false;
  }

  // give the device time to sample (2 fill RGB cycles (100ms per color))
  // well, theoretically, fact is, lowering the delay to 400ms will make it
  // almost always miss blue
  delay(600);

  isl_read_rgb48(color);

  isl_set(ISL_R_COLOR_MODE, ISL_MODE_POWERDOWN);

  return true;
}

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);
  PRINTF("ubirch lights-sensor v2.0\r\n");

  uc_import_ecc_key(&uc_key, device_ecc_key, device_ecc_key_len);

  i2c_init(i2c_config);
  sim800h_init();
  rtc_init();

  rgb48_t rgb;
  status_t status;
  double lat, lon;
  rtc_datetime_t date;
  short int level;
  int voltage;

  while (true) {
    sample_rgb(&rgb, sensitivity);
    PRINTF("-RGB(%s, %lu,%lu,%lu)\r\n", sensitivity == ISL_MODE_375LUX ? "375LUX" : "10KLUX", rgb.red, rgb.green,
           rgb.blue);

    // auto-compensate for brightness
    if (sensitivity == ISL_MODE_375LUX &&
        rgb.red > ISL_375LUX_MAX && rgb.green > ISL_375LUX_MAX && rgb.blue > ISL_375LUX_MAX) {
      sensitivity = ISL_MODE_10KLUX;
      sample_rgb(&rgb, sensitivity);
    } else if (sensitivity == ISL_MODE_10KLUX &&
               rgb.red < ISL_10KLUX_MIN && rgb.green < ISL_10KLUX_MIN && rgb.blue < ISL_10KLUX_MIN) {
      sensitivity = ISL_MODE_375LUX;
      sample_rgb(&rgb, sensitivity);
    }
    PRINTF("+RGB(%s, %lu,%lu,%lu)\r\n", sensitivity == ISL_MODE_375LUX ? "375LUX" : "10KLUX", rgb.red, rgb.green,
           rgb.blue);

    // power on GSM module
    sim800h_enable();
    sim800h_register(6 * TIMEOUT);
    sim800h_gprs_attach(CELL_APN, CELL_USER, CELL_PWD, 6 * TIMEOUT);

    // get battery status and geo coordinates, set time if possible
    sim800h_battery(&status, &level, &voltage, TIMEOUT);
    if (sim800h_location(&status, &lat, &lon, &date, 6 * TIMEOUT)) {
      PRINTF("setting current time from GSM\r\n");
      PRINTF("%04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n",
             date.year, date.month, date.day, date.hour, date.minute, date.second);
      rtc_set(&date);
    }

    // payload structure to be signed
    // Example: '{"r":44,"g":33,"b":22,"s":0,"lat":"12.475886","lon":"51.505264","bat":100,"lps":99999}'
    char payload[128];
    sprintf(payload,
            "{\"r\":%u,\"g\":%u,\"b\":%u,\"s\":%1u,\"la\":\"%f\",\"lo\":\"%f\",\"ba\":%u,\"lp\":%u,\"e\":%u}",
            rgb.red, rgb.green, rgb.blue, sensitivity == ISL_MODE_375LUX ? 0 : 1,
            lat, lon, level, loop_counter, error_flag);

    char imei[16];
    sim800h_imei(imei, TIMEOUT);

    // be aware that you need to free these strings after use
    char *auth_hash = uc_sha512_encoded((const unsigned char *) imei, strnlen(imei, 15));
    char *pub_key_hash = uc_base64_encode(uc_key.p, 32);
    char *payload_hash = uc_ecc_sign_encoded(&uc_key, (const unsigned char *) payload, strlen(payload));

    PRINTF("PUBKEY   : %s\r\n", pub_key_hash);
    PRINTF("AUTH     : %s\r\n", auth_hash);
    PRINTF("SIGNATURE: %s\r\n", payload_hash);

    int message_size = snprintf(NULL, 0,
                                "{\"v\":\"0.0.1\",\"a\":\"%s\",\"k\":\"%s\",\"s\":\"%s\",\"p\":%s}",
                                auth_hash, pub_key_hash, payload_hash, payload);
    char message[message_size + 1];
    sprintf(message, "{\"v\":\"0.0.1\",\"a\":\"%s\",\"k\":\"%s\",\"s\":\"%s\",\"p\":%s}",
            auth_hash, pub_key_hash, payload_hash, payload);

    // free hashes
    free(auth_hash);
    free(pub_key_hash);
    free(payload_hash);

    PRINTF("--MESSAGE (%d)\r\n", strlen(message));
    PRINTF(message);
    PRINTF("\r\n--MESSAGE\r\n");
    size_t response_size;
    int http_status = sim800h_http_post("http://api.ubirch.com/lights",
                                        &response_size, (uint8_t *) message, strlen(message),
                                        5 * TIMEOUT);

    // the message is also dynamically allocated, free it after use
    free(message);

    PRINTF("HTTP: %d [%d byte]\r\n", http_status, response_size);

    uint8_t buffer[response_size];
    sim800h_http_read(buffer, 0, response_size, 2 * TIMEOUT);
    dbg_dump("GSM", buffer, response_size);

    // switch off GSM module
    sim800h_disable();

    rtc_get(&date);
    PRINTF("%04hd-%02hd-%02hd %02hd:%02hd:%02hd DONE.\r\n",
           date.year, date.month, date.day, date.hour, date.minute, date.second);

    delay(5 * 60000);

    rtc_get(&date);
    PRINTF("%04hd-%02hd-%02hd %02hd:%02hd:%02hd WAKEUP.\r\n",
           date.year, date.month, date.day, date.hour, date.minute, date.second);
    loop_counter++;
  }
}

