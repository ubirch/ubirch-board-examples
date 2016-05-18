# ADD_EXECUTABLE
add_executable(wolfssl-benchmark main.c)
target_link_libraries(wolfssl-benchmark ubirch::${MCU}::firmware)
prepare_flash(wolfssl-benchmark)
