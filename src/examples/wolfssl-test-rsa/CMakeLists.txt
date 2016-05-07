# ADD_EXECUTABLE
add_executable(wolfssl-test-rsa main.c)
target_link_libraries(wolfssl-test-rsa board wolfcrypt)
prepare_flash(wolfssl-test-rsa)
