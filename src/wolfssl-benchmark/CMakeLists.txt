# ADD_EXECUTABLE
add_executable(wolfssl-benchmark main.c)
target_link_libraries(wolfssl-benchmark
  ubirch::${BOARD}::firmware
  wolfSSL::${MCU}::wolfcrypt
  )
prepare_flash(TARGET wolfssl-benchmark)
