# ADD_EXECUTABLE
ADD_EXECUTABLE(wolfssl-test main.c)

SET_TARGET_PROPERTIES(wolfssl-test PROPERTIES OUTPUT_NAME "wolfssl-test.elf")

TARGET_LINK_LIBRARIES(wolfssl-test -Wl,--start-group)
TARGET_LINK_LIBRARIES(wolfssl-test ksdk-device-lib)
TARGET_LINK_LIBRARIES(wolfssl-test ksdk-mmcau)
TARGET_LINK_LIBRARIES(wolfssl-test ubirch-firmware)
TARGET_LINK_LIBRARIES(wolfssl-test wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(wolfssl-test m)
TARGET_LINK_LIBRARIES(wolfssl-test c)
TARGET_LINK_LIBRARIES(wolfssl-test gcc)
TARGET_LINK_LIBRARIES(wolfssl-test nosys)
TARGET_LINK_LIBRARIES(wolfssl-test -Wl,--end-group)

# MAP FILE
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=wolfssl-test.map")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker -Map=wolfssl-test.map")

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET wolfssl-test POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:wolfssl-test> $<TARGET_FILE_DIR:wolfssl-test>/wolfssl-test.hex)
ADD_CUSTOM_COMMAND(TARGET wolfssl-test POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:wolfssl-test> $<TARGET_FILE_DIR:wolfssl-test>/wolfssl-test.bin)
ADD_CUSTOM_COMMAND(TARGET wolfssl-test POST_BUILD COMMAND cp $<TARGET_FILE_DIR:wolfssl-test>/wolfssl-test.bin ${CMAKE_CURRENT_SOURCE_DIR})
ADD_CUSTOM_COMMAND(TARGET wolfssl-test POST_BUILD COMMAND cp $<TARGET_FILE_DIR:wolfssl-test>/wolfssl-test.elf ${CMAKE_CURRENT_SOURCE_DIR})
#ADD_CUSTOM_TARGET(burn COMMAND [ -d /Volumes/MBED ] && cp ${CMAKE_CURRENT_SOURCE_DIR}/wolfssl-test.bin /Volumes/MBED DEPENDS wolfssl-test)
