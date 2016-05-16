add_executable(gsm_commands main.c)
target_link_libraries(gsm_commands firmware)
prepare_flash(gsm_commands)
