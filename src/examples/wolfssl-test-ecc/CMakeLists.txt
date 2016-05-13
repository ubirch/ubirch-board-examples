add_executable(wolfssl-test-ecc main.c ecc_keys.h)
target_link_libraries(wolfssl-test-ecc board wolfcrypt dbgutil)
prepare_flash(wolfssl-test-ecc)
