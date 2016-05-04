# ADD_EXECUTABLE
add_executable(wolfssl-benchmark main.c)
target_link_libraries(wolfssl-benchmark board)

set_target_properties(wolfssl-benchmark PROPERTIES OUTPUT_NAME "wolfssl-benchmark.elf")

target_link_libraries(wolfssl-benchmark -Wl,--start-group)
target_link_libraries(wolfssl-benchmark ksdk-device-lib)
target_link_libraries(wolfssl-benchmark ksdk-mmcau)
target_link_libraries(wolfssl-benchmark wolfcrypt)
# SYSTEM LIBRARIES
target_link_libraries(wolfssl-benchmark m)
target_link_libraries(wolfssl-benchmark c)
target_link_libraries(wolfssl-benchmark gcc)
target_link_libraries(wolfssl-benchmark nosys)
target_link_libraries(wolfssl-benchmark -Wl,--end-group)

prepare_flash(wolfssl-benchmark)
