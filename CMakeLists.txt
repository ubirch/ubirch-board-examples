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

add_subdirectory(src/test)
add_subdirectory(src/lpuart)
add_subdirectory(src/rtc)
add_subdirectory(src/wolfssl-test)

add_subdirectory(src/sim800h)
add_subdirectory(src/i2c_scan)
add_subdirectory(src/isl29125)
add_subdirectory(src/ssd1306)
