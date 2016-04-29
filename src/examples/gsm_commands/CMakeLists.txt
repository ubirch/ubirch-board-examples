# ADD_EXECUTABLE
add_executable(gsm_commands main.c)
target_link_libraries(gsm_commands sim800h)

set_target_properties(gsm_commands PROPERTIES OUTPUT_NAME "gsm_commands.elf")

target_link_libraries(gsm_commands -Wl,--start-group)
target_link_libraries(gsm_commands ksdk-device-lib)
target_link_libraries(gsm_commands ksdk-mmcau)
target_link_libraries(gsm_commands ubirch-firmware)
target_link_libraries(gsm_commands wolfcrypt)
# SYSTEM LIBRARIES
target_link_libraries(gsm_commands m)
target_link_libraries(gsm_commands c)
target_link_libraries(gsm_commands gcc)
target_link_libraries(gsm_commands nosys)
target_link_libraries(gsm_commands -Wl,--end-group)


prepare_flash(gsm_commands)
