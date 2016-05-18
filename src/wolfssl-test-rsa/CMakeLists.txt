# ADD_EXECUTABLE
add_executable(wolfssl-test-rsa main.c)
target_link_libraries(wolfssl-test-rsa ubirch::${MCU}::firmware)
prepare_flash(wolfssl-test-rsa)
