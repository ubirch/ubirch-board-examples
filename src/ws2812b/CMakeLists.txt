add_executable(ws2812b main.c)
target_link_libraries(ws2812b ubirch::${BOARD}::firmware)
prepare_flash(TARGET ws2812b)
