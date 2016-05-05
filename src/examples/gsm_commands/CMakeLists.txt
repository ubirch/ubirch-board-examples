add_executable(gsm_commands main.c)
target_link_libraries(gsm_commands board sim800h)
set_target_properties(gsm_commands PROPERTIES OUTPUT_NAME "gsm_commands.elf")
prepare_flash(gsm_commands)
