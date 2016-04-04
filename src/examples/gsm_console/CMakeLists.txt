# ADD_EXECUTABLE
ADD_EXECUTABLE(gsm_console main.c)
target_link_libraries(gsm_console sim800h)

SET_TARGET_PROPERTIES(gsm_console PROPERTIES OUTPUT_NAME "gsm_console.elf")

TARGET_LINK_LIBRARIES(gsm_console -Wl,--start-group)
TARGET_LINK_LIBRARIES(gsm_console ksdk-device-lib)
TARGET_LINK_LIBRARIES(gsm_console ksdk-mmcau)
TARGET_LINK_LIBRARIES(gsm_console ubirch-firmware)
TARGET_LINK_LIBRARIES(gsm_console wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(gsm_console m)
TARGET_LINK_LIBRARIES(gsm_console c)
TARGET_LINK_LIBRARIES(gsm_console gcc)
TARGET_LINK_LIBRARIES(gsm_console nosys)
TARGET_LINK_LIBRARIES(gsm_console -Wl,--end-group)

# MAP FILE
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=gsm_console.map")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker -Map=gsm_console.map")

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET gsm_console POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:gsm_console> $<TARGET_FILE_DIR:gsm_console>/gsm_console.hex)
ADD_CUSTOM_COMMAND(TARGET gsm_console POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:gsm_console> $<TARGET_FILE_DIR:gsm_console>/gsm_console.bin)
ADD_CUSTOM_COMMAND(TARGET gsm_console POST_BUILD COMMAND cp $<TARGET_FILE_DIR:gsm_console>/gsm_console.bin ${CMAKE_CURRENT_SOURCE_DIR})
ADD_CUSTOM_COMMAND(TARGET gsm_console POST_BUILD COMMAND cp $<TARGET_FILE_DIR:gsm_console>/gsm_console.elf ${CMAKE_CURRENT_SOURCE_DIR})
#ADD_CUSTOM_TARGET(burn COMMAND [ -d /Volumes/MBED ] && cp ${CMAKE_CURRENT_SOURCE_DIR}/gsm_console.bin /Volumes/MBED DEPENDS gsm_console)
