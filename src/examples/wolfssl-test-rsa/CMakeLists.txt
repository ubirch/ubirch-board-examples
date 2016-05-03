# ADD_EXECUTABLE
add_executable(wolfssl-test-rsa main.c)
target_link_libraries(wolfssl-test-rsa board)

set_target_properties(wolfssl-test-rsa PROPERTIES OUTPUT_NAME "wolfssl-test-rsa.elf")

target_link_libraries(wolfssl-test-rsa -Wl,--start-group)
target_link_libraries(wolfssl-test-rsa ksdk-device-lib)
target_link_libraries(wolfssl-test-rsa ksdk-mmcau)
target_link_libraries(wolfssl-test-rsa wolfcrypt)
# SYSTEM LIBRARIES
target_link_libraries(wolfssl-test-rsa m)
target_link_libraries(wolfssl-test-rsa c)
target_link_libraries(wolfssl-test-rsa gcc)
target_link_libraries(wolfssl-test-rsa nosys)
target_link_libraries(wolfssl-test-rsa -Wl,--end-group)

prepare_flash(wolfssl-test-rsa)
