# ADD_EXECUTABLE
add_executable(gsm_console main.c)
target_link_libraries(gsm_console board sim800h)

set_target_properties(gsm_console PROPERTIES OUTPUT_NAME "gsm_console.elf")

target_link_libraries(gsm_console -Wl,--start-group)
target_link_libraries(gsm_console ksdk-device-lib)
target_link_libraries(gsm_console ksdk-mmcau)
# SYSTEM LIBRARIES
target_link_libraries(gsm_console m)
target_link_libraries(gsm_console c)
target_link_libraries(gsm_console gcc)
target_link_libraries(gsm_console nosys)
target_link_libraries(gsm_console -Wl,--end-group)

prepare_flash(gsm_console)
