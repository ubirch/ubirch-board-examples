cmake_minimum_required(VERSION 3.0.2)

set(CMAKE_TOOLCHAIN_FILE cmake/ubirch-arm-gcc-toolchain.cmake)

# set board type (ubirch#1 r0.2, FRDM-K82F, FRDM-KL82Z*) - * not yet implemented
# you don't need to set it here, use 'cmake -DBOARD=<board> ..' during setup
if (NOT BOARD)
#  set(BOARD "ubirch#1")
  set(BOARD "FRDM-K82F")
#  set(BOARD "FRDM-KL82Z")
endif ()
message(STATUS "selected board: ${BOARD}")

project(ubirch-board C CXX ASM)

add_definitions(-DNCIODEBUG)

# include special libs
include(cmake/Libraries/mmcau.cmake)
include(cmake/Libraries/wolfssl.cmake)
include(cmake/Libraries/flashutil.cmake)

# board specific code (pins, peripherals etc.)
add_subdirectory(src/board)

# ubirch driver code
# we need to add the include directory here, so it does not get
# lost in the nested scope of add_subdirectory()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/driver)
add_subdirectory(src/driver)

# example code
add_subdirectory(src/examples)

# original pek firmware apps
include(cmake/ubirch-firmware-pek.cmake)
