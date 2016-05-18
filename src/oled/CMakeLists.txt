add_executable(oled main.c)
target_link_libraries(oled ubirch::${MCU}::firmware)
prepare_flash(oled)
