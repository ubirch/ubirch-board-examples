# timer driver
add_library(rtc rtc.c)
target_link_libraries(rtc board)
target_include_directories(rtc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
