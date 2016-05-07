#include <timer.h>
#include <rtc.h>
#include <sim800h.h>
#include <sim800h_ops.h>

#define CELL_ENABLED 1

bool alarm_triggered = false;

void alarm(rtc_datetime_t *date) {
  PRINTF("ALARM! %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n",
         date->year, date->month, date->day, date->hour, date->minute, date->second);
  alarm_triggered = true;
}

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  rtc_datetime_t date = (rtc_datetime_t) {2016, 05, 07, 22, 18, 22};

  if (CELL_ENABLED) {
    sim800h_init();
    sim800h_enable();

    sim800h_register(30000);
    sim800h_gprs_attach("eseye.com", "ubirch", "internet", 30000);

    short int loc_status;
    short int bat_status, bat_level;
    int bat_voltage;
    double lon, lat;

    do {
      sim800h_battery(&bat_status, &bat_level, &bat_voltage, 1000);
      PRINTF("BATTERY: %d%% [%d.%dV] %s\r\n", bat_level, bat_voltage / 1000, bat_voltage % 1000,
             (bat_status == 0 ? "not charging" : (bat_status == 1 ? "charging" : "charged")));
      sim800h_location(&loc_status, &lat, &lon, &date, 30000);
    } while (loc_status);

    PRINTF("GSM: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", date.year, date.month, date.day, date.hour,
           date.minute, date.second);

    sim800h_disable();
  }

  rtc_init();
  rtc_attach(alarm);
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
    PRINTF("The second must be positive value. It will not be echoed.\r\n");
    while (sec < 1) {
      SCANF("%d", &sec);
    }

    // set alarm in n seconds
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
