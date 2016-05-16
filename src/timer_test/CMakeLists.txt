add_executable(timer_test main.c)
target_link_libraries(timer_test board firmware)
prepare_flash(timer_test)
