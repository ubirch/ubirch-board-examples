add_executable(wolfssl-test-ecc main.c)
target_link_libraries(wolfssl-test-ecc
  ubirch::${BOARD}::firmware
  wolfSSL::${MCU}::wolfcrypt
  )
prepare_flash(TARGET wolfssl-test-ecc)
