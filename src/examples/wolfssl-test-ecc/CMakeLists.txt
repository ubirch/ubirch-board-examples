# ADD_EXECUTABLE
add_executable(wolfssl-test-ecc main.c)
target_link_libraries(wolfssl-test-ecc board)

SET_TARGET_PROPERTIES(wolfssl-test-ecc PROPERTIES OUTPUT_NAME "wolfssl-test-ecc.elf")

target_link_libraries(wolfssl-test-ecc -Wl,--start-group)
target_link_libraries(wolfssl-test-ecc ksdk-device-lib)
target_link_libraries(wolfssl-test-ecc ksdk-mmcau)
target_link_libraries(wolfssl-test-ecc wolfcrypt)
# SYSTEM LIBRARIES
target_link_libraries(wolfssl-test-ecc m)
target_link_libraries(wolfssl-test-ecc c)
target_link_libraries(wolfssl-test-ecc gcc)
target_link_libraries(wolfssl-test-ecc nosys)
target_link_libraries(wolfssl-test-ecc -Wl,--end-group)

prepare_flash(wolfssl-test-ecc)
