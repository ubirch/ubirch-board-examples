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

# MAP FILE
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=wolfssl-test-rsa.map")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker -Map=wolfssl-test-rsa.map")

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET wolfssl-test-rsa POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:wolfssl-test-rsa> $<TARGET_FILE_DIR:wolfssl-test-rsa>/wolfssl-test-rsa.hex)
ADD_CUSTOM_COMMAND(TARGET wolfssl-test-rsa POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:wolfssl-test-rsa> $<TARGET_FILE_DIR:wolfssl-test-rsa>/wolfssl-test-rsa.bin)
ADD_CUSTOM_COMMAND(TARGET wolfssl-test-rsa POST_BUILD COMMAND cp $<TARGET_FILE_DIR:wolfssl-test-rsa>/wolfssl-test-rsa.bin ${CMAKE_CURRENT_SOURCE_DIR})
ADD_CUSTOM_COMMAND(TARGET wolfssl-test-rsa POST_BUILD COMMAND cp $<TARGET_FILE_DIR:wolfssl-test-rsa>/wolfssl-test-rsa.elf ${CMAKE_CURRENT_SOURCE_DIR})
#ADD_CUSTOM_TARGET(burn COMMAND [ -d /Volumes/MBED ] && cp ${CMAKE_CURRENT_SOURCE_DIR}/wolfssl-test-rsa.bin /Volumes/MBED DEPENDS wolfssl-test-rsa)
