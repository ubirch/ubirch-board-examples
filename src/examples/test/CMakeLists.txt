# ADD_EXECUTABLE
ADD_EXECUTABLE(ubirch-board main.c)

SET_TARGET_PROPERTIES(ubirch-board PROPERTIES OUTPUT_NAME "ubirch-board.elf")

TARGET_LINK_LIBRARIES(ubirch-board -Wl,--start-group)
TARGET_LINK_LIBRARIES(ubirch-board ksdk-device-lib)
TARGET_LINK_LIBRARIES(ubirch-board ksdk-mmcau)
TARGET_LINK_LIBRARIES(ubirch-board ubirch-firmware)
TARGET_LINK_LIBRARIES(ubirch-board wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(ubirch-board m)
TARGET_LINK_LIBRARIES(ubirch-board c)
TARGET_LINK_LIBRARIES(ubirch-board gcc)
TARGET_LINK_LIBRARIES(ubirch-board nosys)
TARGET_LINK_LIBRARIES(ubirch-board -Wl,--end-group)

# MAP FILE
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=ubirch-board.map")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker -Map=ubirch-board.map")

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET ubirch-board POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:ubirch-board> $<TARGET_FILE_DIR:ubirch-board>/ubirch-board.hex)
ADD_CUSTOM_COMMAND(TARGET ubirch-board POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:ubirch-board> $<TARGET_FILE_DIR:ubirch-board>/ubirch-board.bin)
ADD_CUSTOM_COMMAND(TARGET ubirch-board POST_BUILD COMMAND cp $<TARGET_FILE_DIR:ubirch-board>/ubirch-board.bin ${CMAKE_CURRENT_SOURCE_DIR})
ADD_CUSTOM_COMMAND(TARGET ubirch-board POST_BUILD COMMAND cp $<TARGET_FILE_DIR:ubirch-board>/ubirch-board.elf ${CMAKE_CURRENT_SOURCE_DIR})
#ADD_CUSTOM_TARGET(burn COMMAND [ -d /Volumes/MBED ] && cp ${CMAKE_CURRENT_SOURCE_DIR}/ubirch-board.bin /Volumes/MBED DEPENDS ubirch-board)
