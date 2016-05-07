# timer driver
add_library(timer timer.c)
target_link_libraries(timer board)
target_include_directories(timer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
