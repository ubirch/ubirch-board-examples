cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_TOOLCHAIN_FILE ../ubirch-arm-toolchain/cmake/ubirch-arm-gcc-toolchain.cmake)

# set board type (ubirch#1 r0.2, FRDM-K82F, FRDM-KL82Z*) - * not yet implemented
# you don't need to set it here, use 'cmake -DBOARD=<board> ..' during setup
if (NOT BOARD)
#  set(BOARD "ubirch#1")
  set(BOARD "FRDM-K82F")
#  set(BOARD "FRDM-KL82Z")
endif ()

message(STATUS "selected board: ${BOARD}")
if (BOARD MATCHES "ubirch#1|FRDM-K82F")
  set(MCU "MK82F25615")
elseif (BOARD MATCHES "FRDM-KL82Z")
  set(MCU "MKL82Z7")
endif ()

project(ubirch-board C CXX ASM)

require(PACKAGE ubirch-firmware MCU ${MCU} VERSION 1.0)
require(PACKAGE KinetisSDK MCU ${MCU} VERSION 2.0)
require(PACKAGE wolfSSL MCU ${MCU} VERSION 3.9.0)

# include special libs
include(cmake/Libraries/flashutil.cmake)

# example code
add_subdirectory(src)
