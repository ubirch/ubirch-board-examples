# This is the CMakeLists.txt for the ubirch firmware layer
# It uses the special code for the board, but not the MK82F25615 layer, as it is also available
# in the Kinetis SDK for our specfic MCU.

set(UBIRCH_FIRMWARE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/firmware")

# select all .c files from the board directory and add them to the sources
file(GLOB FIRMWARE_SRCS ${UBIRCH_FIRMWARE_ROOT}/runtime/board/*.c)
ADD_LIBRARY(ubirch-firmware STATIC ${FIRMWARE_SRCS})
INCLUDE_DIRECTORIES(${UBIRCH_FIRMWARE_ROOT}/runtime/board)
