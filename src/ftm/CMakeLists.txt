add_executable(ftm-test main.c)
target_link_libraries(ftm-test ubirch::${BOARD}::firmware)
prepare_flash(TARGET ftm-test)
