add_executable(oled-fractal main.c)
target_link_libraries(oled-fractal ubirch::${BOARD}::firmware)
prepare_flash(TARGET oled-fractal)
