add_executable(timer_test main.c)
set_target_properties(timer_test PROPERTIES OUTPUT_NAME "timer_test.elf")
target_link_libraries(timer_test board timer)
prepare_flash(timer_test)
