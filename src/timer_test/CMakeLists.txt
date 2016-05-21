add_executable(timer_test main.c)
target_link_libraries(timer_test ubirch::${BOARD}::firmware)
prepare_flash(TARGET timer_test)
