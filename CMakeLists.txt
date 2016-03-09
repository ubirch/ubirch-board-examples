cmake_minimum_required(VERSION 3.4)

# configure Kinetis SDK directory and compiler
set(KSDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDK_2.0_MK82FN256xxx15)
set(ENV{ARMGCC_DIR} "/usr/local/Cellar/gcc-arm-none-eabi-49/20150609")
set(CMAKE_TOOLCHAIN_FILE "${KSDK_ROOT}/tools/cmake_toolchain_files/armgcc.cmake")

if (NOT EXISTS "${KSDK_ROOT}/")
  message(FATAL_ERROR "Missing Kinetis SDK directory, please link or copy into libs/ as 'ksdk'")
endif ()

PROJECT(ubirch-board)

include(cmake/kinetis-sdk-2.0.cmake)
include(cmake/ubirch-firmware.cmake)
include(cmake/wolfssl.cmake)

# ADD_EXECUTABLE
ADD_EXECUTABLE(ubirch-board
  src/main.c
  )

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
