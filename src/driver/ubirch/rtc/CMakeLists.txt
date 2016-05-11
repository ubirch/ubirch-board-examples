# timer driver
add_library(rtc rtc.c)
target_link_libraries(rtc board)
