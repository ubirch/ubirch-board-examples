add_executable(wolfssl-test-ecc main.c)
target_link_libraries(wolfssl-test-ecc board wolfcrypt)
set_target_properties(wolfssl-test-ecc PROPERTIES OUTPUT_NAME "wolfssl-test-ecc.elf")
prepare_flash(wolfssl-test-ecc)
