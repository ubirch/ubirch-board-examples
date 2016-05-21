add_executable(gsm_commands main.c)
target_link_libraries(gsm_commands ubirch::${BOARD}::firmware)
prepare_flash(TARGET gsm_commands)
