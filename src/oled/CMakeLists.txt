add_executable(oled main.c)
target_link_libraries(oled board firmware)
prepare_flash(oled)
