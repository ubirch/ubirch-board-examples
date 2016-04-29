# ADD_EXECUTABLE
ADD_EXECUTABLE(wolfssl-test-rsa main.c)

SET_TARGET_PROPERTIES(wolfssl-test-rsa PROPERTIES OUTPUT_NAME "wolfssl-test-rsa.elf")

TARGET_LINK_LIBRARIES(wolfssl-test-rsa -Wl,--start-group)
TARGET_LINK_LIBRARIES(wolfssl-test-rsa ksdk-device-lib)
TARGET_LINK_LIBRARIES(wolfssl-test-rsa ksdk-mmcau)
TARGET_LINK_LIBRARIES(wolfssl-test-rsa ubirch-firmware)
TARGET_LINK_LIBRARIES(wolfssl-test-rsa wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(wolfssl-test-rsa m)
TARGET_LINK_LIBRARIES(wolfssl-test-rsa c)
TARGET_LINK_LIBRARIES(wolfssl-test-rsa gcc)
TARGET_LINK_LIBRARIES(wolfssl-test-rsa nosys)
TARGET_LINK_LIBRARIES(wolfssl-test-rsa -Wl,--end-group)

prepare_flash(wolfssl-test-rsa)
