cmake_minimum_required(VERSION 3.4)

# configure Kinetis SDK directory and compiler
set(KSDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDK_2.0_MK82FN256xxx15)
set(ENV{ARMGCC_DIR} "/usr/local/Cellar/gcc-arm-none-eabi-49/20150609")
set(CMAKE_TOOLCHAIN_FILE "${KSDK_ROOT}/tools/cmake_toolchain_files/armgcc.cmake")

# also add the gdb to our list of tools
find_program(CMAKE_GDB arm-none-eabi-gdb)
find_program(CGDB cgdb)
if (CGDBB_NO_FOUND)
  message(STATUS "For debugging, install cgdb...")
else ()
  message(STATUS "DEBUGGER: ${CGDB} found.")
endif ()

# we need this gdbinit file for flashing directly
# needs "JLinkGDBServer  -if SWD -device MK82FN256xxx15" to be running
set(GDBINIT ${CMAKE_CURRENT_SOURCE_DIR}/.gdbinit)

function(prepare_flash NAME)
  # MAP FILE
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=${NAME}.map")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker -Map=${NAME}.map")

  # create special target that directly flashes
  add_custom_target(${NAME}-flash
    DEPENDS ${NAME}
    COMMAND ${CMAKE_GDB} -x ${GDBINIT}_flash --batch $<TARGET_FILE_DIR:${NAME}>/${NAME}.elf
    COMMAND echo "==== DEBUG COMMAND ====="
    COMMAND echo cgdb -d ${CMAKE_GDB} -x ${GDBINIT} $<TARGET_FILE_DIR:${NAME}>/${NAME}.elf
    COMMAND echo "========================"
    )
endfunction()


if (NOT EXISTS "${KSDK_ROOT}/")
  message(FATAL_ERROR "Missing Kinetis SDK directory, please link or copy into libs/ as '${KSDK_ROOT}'")
endif ()

project(ubirch-board C ASM)

include(cmake/kinetis-sdk-2.0.cmake)
include(cmake/ubirch-firmware.cmake)
include(cmake/wolfssl.cmake)
#include(cmake/ugui.cmake)

add_subdirectory(src/driver)
add_subdirectory(src/examples)
