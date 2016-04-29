# ADD_EXECUTABLE
ADD_EXECUTABLE(wolfssl-benchmark main.c)

SET_TARGET_PROPERTIES(wolfssl-benchmark PROPERTIES OUTPUT_NAME "wolfssl-benchmark.elf")

TARGET_LINK_LIBRARIES(wolfssl-benchmark -Wl,--start-group)
TARGET_LINK_LIBRARIES(wolfssl-benchmark ksdk-device-lib)
TARGET_LINK_LIBRARIES(wolfssl-benchmark ksdk-mmcau)
TARGET_LINK_LIBRARIES(wolfssl-benchmark ubirch-firmware)
TARGET_LINK_LIBRARIES(wolfssl-benchmark wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(wolfssl-benchmark m)
TARGET_LINK_LIBRARIES(wolfssl-benchmark c)
TARGET_LINK_LIBRARIES(wolfssl-benchmark gcc)
TARGET_LINK_LIBRARIES(wolfssl-benchmark nosys)
TARGET_LINK_LIBRARIES(wolfssl-benchmark -Wl,--end-group)

prepare_flash(wolfssl-benchmark)
