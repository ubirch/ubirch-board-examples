add_executable(oled main.c)
target_link_libraries(oled board ssd1306 i2c)
set_target_properties(oled PROPERTIES OUTPUT_NAME "oled.elf")
prepare_flash(oled)
