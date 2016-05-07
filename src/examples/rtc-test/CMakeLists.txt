add_executable(rtc-test main.c)
target_link_libraries(rtc-test board rtc)
prepare_flash(rtc-test)
