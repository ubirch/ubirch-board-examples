# ADD_EXECUTABLE
ADD_EXECUTABLE(wolfssl-test-ecc main.c)

SET_TARGET_PROPERTIES(wolfssl-test-ecc PROPERTIES OUTPUT_NAME "wolfssl-test-ecc.elf")

TARGET_LINK_LIBRARIES(wolfssl-test-ecc -Wl,--start-group)
TARGET_LINK_LIBRARIES(wolfssl-test-ecc ksdk-device-lib)
TARGET_LINK_LIBRARIES(wolfssl-test-ecc ksdk-mmcau)
TARGET_LINK_LIBRARIES(wolfssl-test-ecc ubirch-firmware)
TARGET_LINK_LIBRARIES(wolfssl-test-ecc wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(wolfssl-test-ecc m)
TARGET_LINK_LIBRARIES(wolfssl-test-ecc c)
TARGET_LINK_LIBRARIES(wolfssl-test-ecc gcc)
TARGET_LINK_LIBRARIES(wolfssl-test-ecc nosys)
TARGET_LINK_LIBRARIES(wolfssl-test-ecc -Wl,--end-group)

# MAP FILE
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=wolfssl-test-ecc.map")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker -Map=wolfssl-test-ecc.map")

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET wolfssl-test-ecc POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:wolfssl-test-ecc> $<TARGET_FILE_DIR:wolfssl-test-ecc>/wolfssl-test-ecc.hex)
ADD_CUSTOM_COMMAND(TARGET wolfssl-test-ecc POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:wolfssl-test-ecc> $<TARGET_FILE_DIR:wolfssl-test-ecc>/wolfssl-test-ecc.bin)
ADD_CUSTOM_COMMAND(TARGET wolfssl-test-ecc POST_BUILD COMMAND cp $<TARGET_FILE_DIR:wolfssl-test-ecc>/wolfssl-test-ecc.bin ${CMAKE_CURRENT_SOURCE_DIR})
ADD_CUSTOM_COMMAND(TARGET wolfssl-test-ecc POST_BUILD COMMAND cp $<TARGET_FILE_DIR:wolfssl-test-ecc>/wolfssl-test-ecc.elf ${CMAKE_CURRENT_SOURCE_DIR})
#ADD_CUSTOM_TARGET(burn COMMAND [ -d /Volumes/MBED ] && cp ${CMAKE_CURRENT_SOURCE_DIR}/wolfssl-test-ecc.bin /Volumes/MBED DEPENDS wolfssl-test-ecc)
