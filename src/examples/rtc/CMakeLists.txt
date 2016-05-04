# ADD_EXECUTABLE
add_executable(rtc main.c)
target_link_libraries(rtc board)

set_target_properties(rtc PROPERTIES OUTPUT_NAME "rtc.elf")

target_link_libraries(rtc -Wl,--start-group)
target_link_libraries(rtc ksdk-device-lib)
target_link_libraries(rtc ksdk-mmcau)
# SYSTEM LIBRARIES
target_link_libraries(rtc m)
target_link_libraries(rtc c)
target_link_libraries(rtc gcc)
target_link_libraries(rtc nosys)
target_link_libraries(rtc -Wl,--end-group)

prepare_flash(rtc)
