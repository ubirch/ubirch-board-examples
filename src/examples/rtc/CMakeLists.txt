add_executable(rtc main.c)
target_link_libraries(rtc board)
set_target_properties(rtc PROPERTIES OUTPUT_NAME "rtc.elf")
prepare_flash(rtc)
