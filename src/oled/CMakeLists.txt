add_executable(oled main.c)
target_link_libraries(oled ubirch::${BOARD}::firmware)
prepare_flash(oled)
