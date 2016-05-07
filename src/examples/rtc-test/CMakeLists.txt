add_executable(rtc-test main.c)
target_link_libraries(rtc-test board rtc sim800h)
prepare_flash(rtc-test)
