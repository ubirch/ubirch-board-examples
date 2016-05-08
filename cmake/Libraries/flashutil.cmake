# add the gdb for the macro below
find_program(CMAKE_GDB arm-none-eabi-gdb)
find_program(CGDB cgdb)
if (CGDBB_NO_FOUND)
  message(STATUS "For debugging, install cgdb...")
else ()
  message(STATUS "debugger: ${CGDB} found.")
endif ()

# we need this gdbinit file for flashing directly
# needs "JLinkGDBServer  -if SWD -device MK82FN256xxx15" to be running
set(GDBINIT ${CMAKE_CURRENT_SOURCE_DIR}/.gdbinit)
message(STATUS "gdbinit: ${GDBINIT}")

# create special target that directly flashes
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
  set(MBED_FLASH_DIR /Volumes/MBED)
else ()
  if (NOT MBED_FLASH_DIR)
    message(FATAL_ERROR "Please set MBED_FLASH_DIR to the directory where MBED mounts!")
  endif ()
endif ()
message(STATUS "MBED flash directory: ${MBED_FLASH_DIR}")

macro(prepare_flash NAME)
  # MAP FILE
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=${NAME}.map")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker -Map=${NAME}.map")

  if (BOARD MATCHES "ubirch#1")
    # create special target that directly flashes
    add_custom_target(${NAME}-flash
      DEPENDS ${NAME}
      COMMAND ${CMAKE_GDB} -x ${GDBINIT}_flash --batch $<TARGET_FILE_DIR:${NAME}>/${NAME}.elf
      )
  else ()
    add_custom_command(TARGET ${NAME} POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${NAME}> $<TARGET_FILE_DIR:${NAME}>/${NAME}.bin
      )
    add_custom_target(${NAME}-flash
      DEPENDS ${NAME}
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${NAME}>/${NAME}.bin ${MBED_FLASH_DIR}
      )
  endif ()
  # print out some helpful hints to debug
  add_custom_target(${NAME}-gdb
    DEPENDS ${NAME}
    COMMAND echo "==== DEBUG COMMAND ====="
    COMMAND echo cgdb -d ${CMAKE_GDB} -x ${GDBINIT} $<TARGET_FILE_DIR:${NAME}>/${NAME}.elf
    COMMAND echo "========================")
endmacro()