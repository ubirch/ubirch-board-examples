add_executable(oled main.c ugui/ugui.c)
target_link_libraries(oled ubirch::${BOARD}::firmware)
prepare_flash(TARGET oled)
