# ADD_EXECUTABLE
ADD_EXECUTABLE(gsm_commands main.c)
target_link_libraries(gsm_commands sim800h)

SET_TARGET_PROPERTIES(gsm_commands PROPERTIES OUTPUT_NAME "gsm_commands.elf")

TARGET_LINK_LIBRARIES(gsm_commands -Wl,--start-group)
TARGET_LINK_LIBRARIES(gsm_commands ksdk-device-lib)
TARGET_LINK_LIBRARIES(gsm_commands ksdk-mmcau)
TARGET_LINK_LIBRARIES(gsm_commands ubirch-firmware)
TARGET_LINK_LIBRARIES(gsm_commands wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(gsm_commands m)
TARGET_LINK_LIBRARIES(gsm_commands c)
TARGET_LINK_LIBRARIES(gsm_commands gcc)
TARGET_LINK_LIBRARIES(gsm_commands nosys)
TARGET_LINK_LIBRARIES(gsm_commands -Wl,--end-group)

# MAP FILE
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=gsm_commands.map")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker -Map=gsm_commands.map")

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET gsm_commands POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:gsm_commands> $<TARGET_FILE_DIR:gsm_commands>/gsm_commands.hex)
ADD_CUSTOM_COMMAND(TARGET gsm_commands POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:gsm_commands> $<TARGET_FILE_DIR:gsm_commands>/gsm_commands.bin)
ADD_CUSTOM_COMMAND(TARGET gsm_commands POST_BUILD COMMAND cp $<TARGET_FILE_DIR:gsm_commands>/gsm_commands.bin ${CMAKE_CURRENT_SOURCE_DIR})
ADD_CUSTOM_COMMAND(TARGET gsm_commands POST_BUILD COMMAND cp $<TARGET_FILE_DIR:gsm_commands>/gsm_commands.elf ${CMAKE_CURRENT_SOURCE_DIR})
#ADD_CUSTOM_TARGET(burn COMMAND [ -d /Volumes/MBED ] && cp ${CMAKE_CURRENT_SOURCE_DIR}/gsm_commands.bin /Volumes/MBED DEPENDS gsm_commands)
