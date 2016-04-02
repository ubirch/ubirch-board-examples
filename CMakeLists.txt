cmake_minimum_required(VERSION 3.4)

FIND_PROGRAM(GCC arm-none-eabi-gcc)
message(STATUS ${GCC})

# configure Kinetis SDK directory and compiler
set(KSDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDK_2.0_MK82FN256xxx15)
set(ENV{ARMGCC_DIR} "/usr/local/Cellar/gcc-arm-none-eabi-49/20150609")
set(CMAKE_TOOLCHAIN_FILE "${KSDK_ROOT}/tools/cmake_toolchain_files/armgcc.cmake")

if (NOT EXISTS "${KSDK_ROOT}/")
  message(FATAL_ERROR "Missing Kinetis SDK directory, please link or copy into libs/ as '${KSDK_ROOT}'")
endif ()

PROJECT(ubirch-board C ASM)

include(cmake/kinetis-sdk-2.0.cmake)
include(cmake/ubirch-firmware.cmake)
include(cmake/wolfssl.cmake)
#include(cmake/ugui.cmake)

add_subdirectory(src/driver)
add_subdirectory(src/examples)
