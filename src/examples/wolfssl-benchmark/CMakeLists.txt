# ADD_EXECUTABLE
add_executable(wolfssl-benchmark main.c)
target_link_libraries(wolfssl-benchmark board timer wolfcrypt)
prepare_flash(wolfssl-benchmark)
