add_executable(wolfssl-test-ecc main.c)
target_link_libraries(wolfssl-test-ecc board wolfcrypt dbgutil)
prepare_flash(wolfssl-test-ecc)
