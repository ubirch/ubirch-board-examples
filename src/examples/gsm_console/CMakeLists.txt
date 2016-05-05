add_executable(gsm_console main.c)
target_link_libraries(gsm_console board sim800h)
set_target_properties(gsm_console PROPERTIES OUTPUT_NAME "gsm_console.elf")
prepare_flash(gsm_console)
