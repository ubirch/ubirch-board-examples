#include <board.h>
#include "../../driver/rtc/rtc.h"

bool alarm_triggered = false;

void alarm(rtc_datetime_t *date) {
  PRINTF("ALARM! %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n",
         date->year, date->month, date->day, date->hour, date->minute, date->second);
  alarm_triggered = true;
}


int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  rtc_init();
  rtc_attach(alarm);

  rtc_datetime_t date;

  date.year = 2016U;
  date.month = 04U;
  date.day = 1U;
  date.hour = 23U;
  date.minute = 15;
  date.second = 0;

  rtc_set(&date);

  /* This loop will set the RTC alarm */
  while (1) {
    alarm_triggered = false;

    rtc_get(&date);

    /* print default time */
    PRINTF("Current datetime: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", date.year, date.month, date.day, date.hour,
           date.minute, date.second);

    /* Get alarm time from user */
    uint32_t sec = 0;
    PRINTF("Please input the number of second to wait for alarm \r\n");
    PRINTF("The second must be positive value\r\n");
    while (sec < 1) {
      SCANF("%d", &sec);
    }

    rtc_set_alarm_in(sec);

    // get alarm time
    rtc_get_alarm(&date);

    /* Print alarm time */
    PRINTF("Alarm @ %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n",
           date.year, date.month, date.day,
           date.hour, date.minute, date.second);

    /* Wait until alarm occurs */
    while (!alarm_triggered) {
    }

    PRINTF("Alarm triggered !!!!\r\n");
  }
}
