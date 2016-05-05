cmake_minimum_required(VERSION 3.4)

# configure Kinetis SDK directory and compiler
set(MCU K82F)
set(KSDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDK_2.0_MK82FN256xxx15)
set(CMAKE_TOOLCHAIN_FILE cmake/ubirch-arm-gcc-toolchain.cmake)

# set board type (ubirch#1 r0.2, FRDM-K82F*, FRDM-KL82Z*) - * not yet implemented
if (NOT BOARD)
  set(BOARD "ubirch#1")
#  set(BOARD "FRDM-K82F")
endif ()
message(STATUS "selected board: ${BOARD}")

if (NOT EXISTS "${KSDK_ROOT}/")
  message(FATAL_ERROR "Missing Kinetis SDK directory, please link or copy into libs/ as '${KSDK_ROOT}'")
endif ()

project(ubirch-board C CXX ASM)

# include special libs
include(cmake/ksdk20.cmake)
include(cmake/mmcau.cmake)
include(cmake/wolfssl.cmake)
#include(cmake/ugui.cmake)
include(cmake/flashutil.cmake)

add_subdirectory(src/board)
add_subdirectory(src/driver)
add_subdirectory(src/examples)
