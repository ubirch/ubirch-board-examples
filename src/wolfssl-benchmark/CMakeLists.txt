# ADD_EXECUTABLE
add_executable(wolfssl-benchmark main.c)
target_link_libraries(wolfssl-benchmark ubirch::${BOARD}::firmware)
prepare_flash(TARGET wolfssl-benchmark)
