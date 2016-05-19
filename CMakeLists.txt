cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_TOOLCHAIN_FILE ../ubirch-meta/ubirch-arm-toolchain/cmake/ubirch-arm-gcc-toolchain.cmake)

# set board type (ubirch-1, FRDM-K82F, FRDM-KL82Z*) - * not yet implemented
# you don't need to set it here, use 'cmake -DBOARD=<board> ..' during setup
if (NOT BOARD)
#  set(BOARD "ubirch-1")
  set(BOARD "FRDM-K82F")
#  set(BOARD "FRDM-KL82Z")
endif ()

project(ubirch-board C CXX ASM)

require(PACKAGE ubirch BOARD ${BOARD} VERSION 1.0)

# include special libs
include(cmake/Libraries/flashutil.cmake)

# example code
add_subdirectory(src)
