# ADD_EXECUTABLE
add_executable(wolfssl-test-rsa main.c)
target_link_libraries(wolfssl-test-rsa board wolfcrypt)
set_target_properties(wolfssl-test-rsa PROPERTIES OUTPUT_NAME "wolfssl-test-rsa.elf")
prepare_flash(wolfssl-test-rsa)
