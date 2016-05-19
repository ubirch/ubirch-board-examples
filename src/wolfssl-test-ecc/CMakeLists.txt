add_executable(wolfssl-test-ecc main.c)
target_link_libraries(wolfssl-test-ecc ubirch::${BOARD}::firmware)
prepare_flash(wolfssl-test-ecc)
