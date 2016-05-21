cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_TOOLCHAIN_FILE ../ubirch-meta/ubirch-arm-toolchain/cmake/ubirch-arm-gcc-toolchain.cmake)

# set board type (ubirch1r02, FRDM-K82F, FRDM-KL82Z*) - * not yet implemented
# you don't need to set it here, use 'cmake -DBOARD=<board> ..' during setup
if (NOT BOARD)
#  set(BOARD "ubirch1r02")
  set(BOARD "FRDM-K82F")
#  set(BOARD "FRDM-KL82Z")
endif ()

project(ubirch-board C CXX ASM)
add_definitions(-D_GNU_SOURCE)

require(PACKAGE ubirch BOARD ${BOARD} VERSION 1.0 BUILD_TYPE Release)

# example code
add_subdirectory(src)
