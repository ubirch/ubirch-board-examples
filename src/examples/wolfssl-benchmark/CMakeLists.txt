# ADD_EXECUTABLE
add_executable(wolfssl-benchmark main.c)
target_link_libraries(wolfssl-benchmark board timer wolfcrypt)
set_target_properties(wolfssl-benchmark PROPERTIES OUTPUT_NAME "wolfssl-benchmark.elf")
prepare_flash(wolfssl-benchmark)
