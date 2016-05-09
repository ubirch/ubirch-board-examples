cmake_minimum_required(VERSION 3.4)

set(CMAKE_TOOLCHAIN_FILE cmake/ubirch-arm-gcc-toolchain.cmake)

# set board type (ubirch#1 r0.2, FRDM-K82F*, FRDM-KL82Z*) - * not yet implemented
if (NOT BOARD)
#  set(BOARD "ubirch#1")
  set(BOARD "FRDM-K82F")
#  set(BOARD "FRDM-KL82Z")
endif ()
message(STATUS "selected board: ${BOARD}")

project(ubirch-board C CXX ASM)

# include special libs
include(cmake/Libraries/mmcau.cmake)
include(cmake/Libraries/wolfssl.cmake)
include(cmake/Libraries/flashutil.cmake)

add_subdirectory(src/board)
add_subdirectory(src/driver)
add_subdirectory(src/examples)

# original pek firmware apps
include(cmake/ubirch-firmware-pek.cmake)
