add_executable(oled main.c)
target_link_libraries(oled board timer ssd1306 i2c)
prepare_flash(oled)
