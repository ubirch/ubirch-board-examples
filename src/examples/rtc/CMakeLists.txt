add_executable(rtc main.c)
target_link_libraries(rtc board)
prepare_flash(rtc)
