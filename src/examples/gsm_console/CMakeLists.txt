add_executable(gsm_console main.c)
target_link_libraries(gsm_console board sim800h)
prepare_flash(gsm_console)
