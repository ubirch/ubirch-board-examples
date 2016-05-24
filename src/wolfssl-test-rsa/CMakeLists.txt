# ADD_EXECUTABLE
add_executable(wolfssl-test-rsa main.c)
target_link_libraries(wolfssl-test-rsa
  ubirch::${BOARD}::firmware
  wolfSSL::${MCU}::wolfcrypt
  )
prepare_flash(TARGET wolfssl-test-rsa)
